// Sound (.Cpp)
// ------------

#include <vector>
#include <list>
#include <sstream>
#include <string.h>
#include "raylib.h"
#include "time.h"

using namespace std;

#include "bridge.hpp"
#include "game.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "config.hpp"
#include "board.hpp"
#include "piece.hpp"

#include "net.hpp"
#include "chat.hpp"
#include "wizard.hpp"
#include "panel.hpp"
#include "spell.hpp"
#include "ai.hpp"
#include "effect.hpp"
#include "options.hpp"
#include "sound.hpp"

struct sound_t sound;
struct music_t music;

bool setup_sound_system(void)
{
 log("Setting up sounds..");

 InitAudioDevice();

 load_all_memory_sounds();

 music.volume.current = 0.0;
 music.volume.target = 1.0;
 music.volume.speed = 0.005;

 return true;
}

bool load_music(void)
{
 char name[MAX_STRING], text[MAX_STRING];

 sprintf(name, "text_files\\title_music.ini");
 sprintf(name, "%s", GetFullPath(name));
 
 if (!set_config_file_new(CONFIG_TITLE_MUSIC, name, false))
 {
	log("Can't open title_music config file!");
  return false;
 }
 
 if (IsMusicValid(music.mod))
 {
	StopMusicStream(music.mod);
 }

 //music.mod = NULL;
 //music.mod = new Sound;
 
 find_random_line_from_text_config(CONFIG_TITLE_MUSIC, text);
 log("Loading Music: %s", text);

 sprintf(name, "Music\\%s.ogg", text);
 sprintf(name, "%s", GetFullPath(name));

 music.mod = LoadMusicStream(name);
 if (!IsMusicValid(music.mod) )
 {
	log("Couldn't load music from : %s", name);
	return false;
 }
 log("");

//myMusic->playModule( true ) ; //play and loop
 return true;
}

void play_music(void)
{
 if (IsMusicValid(music.mod))
 {
	StopMusicStream(music.mod);
	PlayMusicStream(music.mod);
	
	//music.volume.current = 0.0;
	music.volume.target = 1.0;
	wait_time(150);
 }
}

void do_music_logic(void)
{
 int c;

 do_alpha_logic(&music.volume);
 if (IsMusicValid(music.mod))
 {
UpdateMusicStream(music.mod);
  find_option_choice_variables(CONFIG_OPTIONS, "SOUND", "MUSIC VOLUME", &c, NULL, NULL, NULL);
  SetMusicVolume(music.mod, music.volume.current * c/100.0f);

  c = find_current_option_choice(CONFIG_OPTIONS, "SOUND", "SOUND BALANCE");

	if (c == 0) SetMusicPan(music.mod,0);
    if (c == 1) SetMusicPan(music.mod,0.5);
	if (c == 2) SetMusicPan(music.mod, 1.0);
 }

 // Switched away?
 if (!CheckWindowFocus())
 {
  if (IsMusicValid(music.mod) && IsMusicStreamPlaying(music.mod))
  {
   PauseMusicStream(music.mod);
  }
 }
 else
 {
  // No, but have we just switched back in?
  if (IsMusicValid(music.mod) && !IsMusicStreamPlaying(music.mod))
	{
	  ResumeMusicStream(music.mod);
	}
 }
}

bool load_all_memory_sounds(void)
{
 int s;
 char name[MAX_STRING], filepath[MAX_STRING];

 // We leave our first entry as free for one off loaded sounds..
 sound.available_sounds = 1;

 sprintf(name, "Sounds\\Memory");
 sprintf(filepath, "%s", GetFullPath(name));
 //KMiscTools::enumerateFolder( filepath , load_sound ) ;

 FilePathList list = LoadDirectoryFiles(filepath);
 for (int i = 0; i < list.count; i++)
 {
	 if (!load_sound(list.paths[i], false, NULL)) break;
 }
 UnloadDirectoryFiles(list);

 log("Loaded %d sounds into memory..", sound.available_sounds);

 return true;
}

 bool load_sound( char *f , bool isFolder, void *userData )
 {
  char name[MAX_STRING];

  if (isFolder) return true; // Skip folders.

	sprintf(name, "%s", GetFileName(f));
    //sprintf(name, "%s", GetFullPath(name));

	sound.sample[sound.available_sounds] = LoadSound(f);

	if (!IsSoundValid(sound.sample[sound.available_sounds])) return true; // Can't load this sound.

	sprintf(sound.name[sound.available_sounds], "%s", name);
  sound.available_sounds++;

  //log("%s, %s", name, f);

	if (sound.available_sounds >= MAX_SOUNDS) return false; // Can't load any more sounds..

  return true;
 }

 int return_sound_number(char *name)
 {
  int s;
  char new_name[MAX_STRING];

	for (s = 0 ; s < sound.available_sounds ; s++)
	{
	 sprintf(new_name, "%s.wav", name);
   if (stricmp(new_name, sound.name[s]) == 0) return s;

	 sprintf(new_name, "%s.ogg", name);
   if (stricmp(new_name, sound.name[s]) == 0) return s;
	}

	return -1; // We couldn't find this sound!
 }

int play_sound(char *name, bool wait)
{
 int s, v, b;
	
 s = return_sound_number(name);
 //log("name: %s, s: %d", name, s);
 if (s == -1) return -1; // Couldn't find sound name!

 find_option_choice_variables(CONFIG_OPTIONS, "SOUND", "SOUND EFFECTS VOLUME", &v, NULL, NULL, NULL);
 if (v <= 0) return s;

 PlaySound(sound.sample[s]);
 SetSoundVolume(sound.sample[s],v/100.0);

 b = find_current_option_choice(CONFIG_OPTIONS, "SOUND", "SOUND BALANCE");

 if (b == 0) SetSoundPan(sound.sample[s],0);
 if (b == 1) SetSoundPan(sound.sample[s],0.5);
 if (b == 2) SetSoundPan(sound.sample[s],1.0);

 if (wait && !game.AI_debug)
 do
 {
  wait_time(1);
 } while (IsSoundPlaying(sound.sample[s]));

 return s;
}

 int request_sound_effect(char *specific, char *general, char *behaviour, bool wait)
 {
  bool ret;
	int c, choices;
  char name[MAX_STRING], var[MAX_STRING], result[MAX_STRING];

	sprintf(name, "%s..%s", specific, behaviour);
	//log("name: %s", name);

  jump:
	GetConfigString(CONFIG_SOUNDS, name, "SAMPLE 1", result, MAX_STRING);
	if (strlen(result) == 0) GetConfigString(CONFIG_SOUNDS, name, "GOTO 1", result, MAX_STRING);

	// Did we find anything with our specific request?
	if (strlen(result) == 0)
	{
	 // No. So, let's try our general request..
   sprintf(name, "%s..%s", general, behaviour);
	 GetConfigString(CONFIG_SOUNDS, name, "SAMPLE 1", result, MAX_STRING);
	 if (strlen(result) == 0) GetConfigString(CONFIG_SOUNDS, name, "GOTO 1", result, MAX_STRING);
	}

	if (strlen(result) == 0) return -1; // Couldn't find any sample options.

	// Ok, we've found at least one option, now let's see how many choices there are..
  c = 0;
	do
	{
	 c++;
   sprintf(var, "SAMPLE %d", c);
   GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);

	 // Did we find a Sample entry?
	 if (strlen(result) == 0)
	 {
		// No? Maybe a Goto jump instead?
    sprintf(var, "GOTO %d", c);
    GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);
	 }
	} while (strlen(result) > 0);

	// Number of choices found.
	choices = c - 1;

	//log("choices: %d", choices);

	c = IRand(1, choices);
  sprintf(var, "SAMPLE %d", c);
  GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);

	// Not a sample?
	if (strlen(result) == 0)
	{
   sprintf(var, "GOTO %d", c);
   GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);
	
	 // It's a goto instead..
	 if (strlen(result) > 0)
	 {
	  sprintf(name, "%s", result);
		//log("GOTO : %s", name);
		goto jump; // Repick a sample..
	 }
	}

	// Play sound
	return play_sound(result, wait);
 }