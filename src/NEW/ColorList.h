#include <list>
#include "Color.h"
using namespace std;
class ColorList{
	private:
		list::Color working;
		list::Color exhaust;
	public:
		ColorList();
		void addColor();
		Color peek();
		void next();
		bool isEmpty();
		void generateSinCurve(unsigned int wavelength, colorPair pair, unsigned int delay_ns);
		void generateFade(unsigned int wavelength, colorPair pair, unsigned int delay_ns);
		static int sin_color_cycle(float time, float wavelength, int start, int end);
		static int color_fade(float time, float wavelength, unsigned int start, unsigned int end);
};
#include "ColorList.cc"
