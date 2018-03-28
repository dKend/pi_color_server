#define limit(x) x%256
Color::Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int brightness, unsigned int delay){
	this.red = red;
	this.green = green;
	this.blue = blue;
	this.brightness = brightness;
	this.delay = delay;
}
unsigned int Color::getRed(){
	return red;
}
unsigned int Color::getGreen(){
	return green;
}
unsigned int Color::getBlue(){
	return blue;
}
unsigned int Color::getBrightness(){
	return brightness;
}
unsigned int Color::getDelay(){
	return delay;
}
void Color::setRed(unsigned int red){
	this.red = limit(red);
}
void Color::setGreen(unsigned int green){
	this.green = limit(green);
}
void Color::setBlue(unsigned int blue){
	this.blue = limit(blue);
}
void Color::setBrightness(unsigned int brightness){
	this.brightness = limit(brightness);
}
void Color::setDelay(unsigned int delay){
	this.delay = delay;
}
void Color::setColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int brightness, unsigned int delay){
	setRed(red);
	setGreen(green);
	setBlue(blue);
	setBrightness(brightness);
	setDelay(delay);
}

