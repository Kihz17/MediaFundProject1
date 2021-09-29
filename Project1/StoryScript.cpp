#include "StoryScript.h"


	int maxSteps = 6;
	StoryStep story[7];
	int currentStep = 0;
	
	StoryScript::StoryScript()
	{
		story[0] = StoryStep();
		story[0].description = "Once Upon the time...";
		story[0].theme = "background_crowd_people_chatter_loop_02.wav";
		story[0].effects = { "bird_small_song_call_chirp_02.wav" };

		story[1] = StoryStep();
		story[1].description = "In the calm forest of Greenwood there was a gnome called Chepart ...";

		story[2] = StoryStep();
		story[2].description = "He lived in the city of Hazelward over the top of the trees ...";

		story[3] = StoryStep();
		story[3].description = "He loved how the people there was so festive ....";

		story[4] = StoryStep();
		story[4].description = "Chepart use to enjoy fishing at Crystal lake every afternoon ...";
		story[4].theme = "music_calm_green_lake_serenade.wav";
		story[4].effects = { "crickets_chirping_night_ambience_loop.wav", "swamp_bayou_frogs_birds_daytime_loop1.wav" };
		story[4].waitTime = 5;

		story[5] = StoryStep();
		story[5].description = "One day he was returned from the lake and had a strange feeling ...";
		story[5].effects = { "fantasy_jungle_forrest_loop_01.wav" };

		story[6] = StoryStep();
		story[6].description = "When he arrived at Hazelward, everyone in the city have become a stone...";
		story[6].theme = "cinematic_LowDrone1.wav";
		story[6].effects = { "bird_crow_call_caw_squawk_01.wav", "shimmer_sparkle_loop_02.wav", "music_cinematic_reveal.wav", "cinematic_deep_low_whoosh_impact_02.wav" };
		story[6].waitTime = 7;
		story[6].isLast = 1;

	}

	StoryScript::~StoryScript()
	{
	}

	StoryStep StoryScript::GetNextLine()
	{
		StoryStep next = story[currentStep];
		currentStep += 1;
		return next;
	}

