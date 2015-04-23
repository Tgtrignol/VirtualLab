#include <btBulletDynamicsCommon.h> //Bullet library

class debug : public btIDebugDraw {

	int m_debugMode;

public:
	debug();
	~debug();

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void setDebugMode(int debugMode);
	void draw3dText(const btVector3& location, const char* textString);
	int getDebugMode() const { return m_debugMode; }

};
