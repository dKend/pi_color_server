using namespace std;
class Color {
	private:
	
		/*
			Unsigned int which stores a value in the range 0-255 and corresponds
			to the red value of a RGB color code.
		*/
		unsigned int red;
		
		/*
			Unsigned int which stores a value in the range 0-255 and corresponds
			to the green value of a RGB color code.
		*/
		unsigned int green;
		
		/*
			Unsigned int which stores a value in the range 0-255 and corresponds
			to the blue value of a RGB color code.
		*/
		unsigned int blue;
		
		/*
			Unsigned int which stores a value in the range 0-255 and corresponds
			to the alpha value of a RGB color code.
		*/
		unsigned int brightness;
		
		/*
			Unsigned int which indicates how long in nanoseconds the color 
			instance will be applied to the LED strip.
		*/
		unsigned int delay;
		
	public:
		/*
		*	Constructor for the Color class.
		*	<p>
		*		Takes five unsigned integers as arguments and uses them to 
		*		create an instance of the color class.
		*	</p>
		*	@param unsigned int red, integer in range 0-255.
		*	@param unsigned int green, above.
		*	@param unsigned int blue, above.
		*	@param unsigned int brightness, above.
		*	@param unsigned int delay, time in nanoseconds; how long the led 
		*	strip will have this color.
		*/
		Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int brightness, unsigned int delay);
		
		/*
		*	Getter for the value of the red field.
		*	@return the value of the red field.
		*/
		unsigned int getRed();
		
		/*
		*	Getter for the value of the green field.
		*	@return the value of the green field.
		*/
		unsigned int getGreen();
		
		/*
		*	Getter for the value of the blue field.
		*	@return the value of the blue field.
		*/
		unsigned int getBlue();
		
		/*
		*	Getter for the value of the brightness field.
		*	@return the value of the brightness field.
		*/
		unsigned int getBrightness();
		
		/*
		*	Getter for the value of the delay field.
		*	@return the value of the delay field.
		*/
		unsigned int getDelay();
		
		/*
		*	Setter for the value of the red field.
		*	@param unsigned int red, integer in range 0-255.
		*/
		void setRed(unsigned int red);
		
		/*
		*	Setter for the value of the green field.
		*	@param unsigned int green, integer in range 0-255.
		*/
		void setGreen(unsigned int green);
		
		/*
		*	Setter for the value of the blue field.
		*	@param unsigned int blue, integer in range 0-255.
		*/
		void setBlue(unsigned int blue);
		
		/*
		*	Setter for the value of the brightness field.
		*	@param unsigned int brightness, integer in range 0-255.
		*/
		void setBrightness(unsigned int brightness);
		
		/*
		*	Setter for the value of the delay field.
		*	@param unsigned int delay, time in nanoseconds; how long the led 
		*	strip will have this color.
		*/
		void setDelay(unsigned int delay);
		
		/*
		*	Setter which takes four unsigned integers: red, green, blue, and
		*	brightness, and sets the corresponding fields within the 
		*	color instance
		*	@param unsigned int red, integer in range 0-255.
		*	@param unsigned int green, above.
		*	@param unsigned int blue, above.
		*	@param unsigned int brightness, above.
		*	strip will have this color.
		*/
		void setColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int brightness);
		
		/*
			Compares the values of the calling color instance to another passed 
			color instance.
			@param Color other, another color instance which is compared to the 
			calling color.
			@return true if each of the fields of the calling color instance are 
			equivalent to the corresponding fields of the other color instance.
		*/
		bool equals(Color other);
};
#include "Color.cc"
