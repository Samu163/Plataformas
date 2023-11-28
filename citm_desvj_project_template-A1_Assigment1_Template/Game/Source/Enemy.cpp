//#include "enemy.h"
//#include "app.h"
//#include "textures.h"
//#include "audio.h"
//#include "input.h"
//#include "render.h"
//#include "scene.h"
//#include "log.h"
//#include "point.h"
//#include "physics.h"
//
//
//Enemy::Enemy(int x, int y) : position(x, y)
//{
//	spawnPos = position;
//}
//
//enemy::~enemy()
//{
//	if (receivedmg != nullptr)
//		receivedmg->pendingtodelete = true;
//	if (afflictdmg != nullptr)
//		afflictdmg->pendingtodelete = true;
//}
//
//const PhysBody* enemy::getcollider() const
//{
//	return receivedmg;
//}
//
//void enemy::update()
//{
//	if (currentanim != nullptr)
//		currentanim->update();
//
//	if (receivedmg != nullptr)
//		receivedmg->setpos(position.x, position.y);
//}
//
//void enemy::draw()
//{
//	if (currentanim != nullptr)
//		app->render->drawtexture(texture, position.x - 45, position.y - 40, isflipped, &currentanimation->getcurrentframe());
//}
//
//void enemy::oncollision(collider* collider)
//{
//	
//}
//
//void enemy::settodelete()
//{
//	pendingtodelete = true;
//	if (receivedmg != nullptr)
//		receivedmg->pendingtodelete = true;
//	if (afflictdmg != nullptr)
//		afflictdmg->pendingtodelete = true;
//}