#include <sdl.h>
#include <vector>



namespace graphics {
	struct fps {
	private:
		// How many frames time values to keep
		// The higher the value the smoother the result is...
		// Don't make it 0 or less :)
		int FRAME_VALUES;
		std::vector<float> frame_times;

		// Last calculated SDL_GetTicks
		Uint32 frameTimeLast;

		// total frames rendered
		std::int64_t frameCount;

		std::int64_t everyXFrames = 60;

		// the value you want to output
		float framesPerSecond;



	public:

		// // Initiliase FPS
		fps() :
			frameTimeLast(0),
			frameCount(0),
			framesPerSecond(0.0f),
			FRAME_VALUES(10)
		{
			for (int i = 0; i < FRAME_VALUES; ++i) {
				frame_times.push_back(0.0f);
			}
		}

		fps(int frame_values) :
			frameTimeLast(0),
			frameCount(0),
			framesPerSecond(0.0f),
			FRAME_VALUES(frame_values)
		{
			for (int i = 0; i < FRAME_VALUES; ++i) {
				frame_times.push_back(0.0f);
			}
		}


		// Initialise everything to 0
		//void init() {
		//	frameTimeLast = 0;
		//	frameCount = 0;
		//	framesPerSecond = 0.0f;
		//}
		template <typename _time>
		void think(_time s_delta_time) {
			Uint32 frame_timesIndex;
			Uint32 getticks;
			Uint32 count;
			Uint32 i;
			_time average_time = 0;

			// // frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
			// // This value rotates back to 0 after it hits FRAME_VALUES.
			//frametimesindex = framecount % FRAME_VALUES;

			frame_times.pop_back();
			auto it = frame_times.begin();
			frame_times.insert(it, 1 / s_delta_time);

			for (auto i : frame_times) {
				average_time += i;
			}
			average_time /= FRAME_VALUES;

			if (--everyXFrames <= 0) {
				std::cout << " Average FPS: " << average_time << "\r";
				everyXFrames += int(average_time);
			}
			// // Increment frame count
			++frameCount;


		}

	};
}