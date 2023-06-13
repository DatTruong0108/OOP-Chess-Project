#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include "GameSound.h"
#include "Utility.h"

using namespace sf;
using namespace std;

namespace utilities {
    class AudioPlayer {
    private:

        static Sound _sound; //sound object

        static map<GameSound, SoundBuffer> _bufferMap; //sound buffer map
    protected:

    public:
        AudioPlayer(); //default constructor

        ~AudioPlayer(); //destructor

        static void loadSound(); //load different sound buffers

        static void playSound(); //resume playing this sound

        static void playSound(GameSound soundType); //play a new type of sound

        static void pauseSound(); //pause the sound

        static void stopSound(); //stop the sound
    };
}
