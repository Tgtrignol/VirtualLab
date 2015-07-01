#ifndef TEXT_REPRESENTATION_H
#define TEXT_REPRESENTATION_H

class TextRepresentation {

public:
	TextRepresentation();
	~TextRepresentation();

	void drawNotes(const char *text, int length);
	void drawText(const char*, int, int, int, int);
	void draw();
	void init();
};

#endif