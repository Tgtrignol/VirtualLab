#include "debug.h"
#include <GL/glew.h>

debug::debug() {

}

debug::~debug() {

}

void debug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {

	glBegin(GL_LINES);
	glColor3f(color.getX(), color.getY(), color.getZ());
	glVertex3d(from.getX(), from.getY(), from.getZ());
	glColor3f(color.getX(), color.getY(), color.getZ());
	glVertex3d(to.getX(), to.getY(), to.getZ());
	glEnd();

}

void debug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
void debug::reportErrorWarning(const char* warningString) {}
void debug::setDebugMode(int debugMode) {}
void debug::draw3dText(const btVector3& location, const char* textString) {}