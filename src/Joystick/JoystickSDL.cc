/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "JoystickSDL.h"
#include "MultiVehicleManager.h"
#include "QGCLoggingCategory.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

JoystickSDL::JoystickSDL(const QString& name, int axisCount, int buttonCount, int hatCount, int index, bool isGameController)
    : Joystick(name,axisCount,buttonCount,hatCount)
    , _isGameController(isGameController)
    , _index(index)
{
    // qCDebug(JoystickLog) << Q_FUNC_INFO << this;

    if(_isGameController) _setDefaultCalibration();
}

JoystickSDL::~JoystickSDL()
{
    // qCDebug(JoystickLog) << Q_FUNC_INFO << this;
}

bool JoystickSDL::init(void) {
    SDL_SetMainReady();
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) < 0) {
        SDL_JoystickEventState(SDL_DISABLE);
        qCWarning(JoystickLog) << "Couldn't initialize SimpleDirectMediaLayer:" << SDL_GetError();
        return false;
    }
    _loadGameControllerMappings();
    return true;
}

QMap<QString, Joystick*> JoystickSDL::discover() {
    static QMap<QString, Joystick*> ret;

    QMap<QString,Joystick*> newRet;

    // Load available joysticks

    qCDebug(JoystickLog) << "Available joysticks";

    for (int i=0; i<SDL_NumJoysticks(); i++) {
        QString name = SDL_JoystickNameForIndex(i);

        if (!ret.contains(name)) {
            int axisCount, buttonCount, hatCount;
            bool isGameController = SDL_IsGameController(i);

            if (SDL_Joystick* sdlJoystick = SDL_JoystickOpen(i)) {
                SDL_ClearError();
                axisCount = SDL_JoystickNumAxes(sdlJoystick);
                buttonCount = SDL_JoystickNumButtons(sdlJoystick);
                hatCount = SDL_JoystickNumHats(sdlJoystick);
                if (axisCount < 0 || buttonCount < 0 || hatCount < 0) {
                    qCWarning(JoystickLog) << "\t libsdl error parsing joystick features:" << SDL_GetError();
                }
                SDL_JoystickClose(sdlJoystick);
            } else {
                qCWarning(JoystickLog) << "\t libsdl failed opening joystick" << qPrintable(name) << "error:" << SDL_GetError();
                continue;
            }

            qCDebug(JoystickLog) << "\t" << name << "axes:" << axisCount << "buttons:" << buttonCount << "hats:" << hatCount << "isGC:" << isGameController;

            // Check for joysticks with duplicate names and differentiate the keys when necessary.
            // This is required when using an Xbox 360 wireless receiver that always identifies as
            // 4 individual joysticks, regardless of how many joysticks are actually connected to the
            // receiver. Using GUID does not help, all of these devices present the same GUID.
            QString originalName = name;
            uint8_t duplicateIdx = 1;
            while (newRet[name]) {
                name = QString("%1 %2").arg(originalName).arg(duplicateIdx++);
            }

            newRet[name] = new JoystickSDL(name, qMax(0,axisCount), qMax(0,buttonCount), qMax(0,hatCount), i, isGameController);
        } else {
            newRet[name] = ret[name];
            JoystickSDL *j = static_cast<JoystickSDL*>(newRet[name]);
            if (j->index() != i) {
                j->setIndex(i); // This joystick index has been remapped by SDL
            }
            // Anything left in ret after we exit the loop has been removed (unplugged) and needs to be cleaned up.
            // We will handle that in JoystickManager in case the removed joystick was in use.
            ret.remove(name);
            qCDebug(JoystickLog) << "\tSkipping duplicate" << name;
        }
    }

    if (!newRet.count()) {
        qCDebug(JoystickLog) << "\tnone found";
    }

    ret = newRet;
    return ret;
}

void JoystickSDL::_loadGameControllerMappings(void) {
    QFile file(":/db/mapping/joystick/gamecontrollerdb.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Couldn't load GameController mapping database.";
        return;
    }

    QTextStream s(&file);

    while (!s.atEnd()) {
        SDL_GameControllerAddMapping(s.readLine().toStdString().c_str());
    }
}

bool JoystickSDL::_open(void) {
    if ( _isGameController ) {
        sdlController = SDL_GameControllerOpen(_index);
        sdlJoystick = SDL_GameControllerGetJoystick(sdlController);
    } else {
        sdlJoystick = SDL_JoystickOpen(_index);
    }

    if (!sdlJoystick) {
        qCWarning(JoystickLog) << "SDL_JoystickOpen failed:" << SDL_GetError();
        return false;
    }

    qCDebug(JoystickLog) << "Opened joystick at" << sdlJoystick;

    return true;
}

void JoystickSDL::_close(void) {
    if (sdlJoystick == nullptr) {
        qCDebug(JoystickLog) << "Attempt to close null joystick!";
        return;
    }

    qCDebug(JoystickLog) << "Closing" << SDL_JoystickName(sdlJoystick) << "at" << sdlJoystick;

    if (_isGameController) {
        SDL_GameControllerClose(sdlController);
    } else {
        SDL_JoystickClose(sdlJoystick);
    }

    sdlJoystick   = nullptr;
    sdlController = nullptr;
}

bool JoystickSDL::_update(void)
{
    if (_isGameController) {
        SDL_GameControllerUpdate();
    } else {
        SDL_JoystickUpdate();
    }
    return true;
}

bool JoystickSDL::_getButton(int i) {
    if (_isGameController) {
        return SDL_GameControllerGetButton(sdlController, SDL_GameControllerButton(i)) == 1;
    } else {
        return SDL_JoystickGetButton(sdlJoystick, i) == 1;
    }
}

int JoystickSDL::_getAxis(int i) {
    if (_isGameController) {
        return SDL_GameControllerGetAxis(sdlController, SDL_GameControllerAxis(i));
    } else {
        return SDL_JoystickGetAxis(sdlJoystick, i);
    }
}

bool JoystickSDL::_getHat(int hat, int i) {
    uint8_t hatButtons[] = {SDL_HAT_UP,SDL_HAT_DOWN,SDL_HAT_LEFT,SDL_HAT_RIGHT};
    if (i < int(sizeof(hatButtons))) {
        return (SDL_JoystickGetHat(sdlJoystick, hat) & hatButtons[i]) != 0;
    }
    return false;
}
