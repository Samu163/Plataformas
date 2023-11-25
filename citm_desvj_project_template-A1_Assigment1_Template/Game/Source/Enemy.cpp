//#include "Enemy.h"
//#include "App.h"
//#include "Textures.h"
//#include "Audio.h"
//#include "Input.h"
//#include "Render.h"
//#include "Scene.h"
//#include "Log.h"
//#include "Point.h"
//#include "Physics.h"
//
//
//Enemy::Enemy(int x, int y) : position(x, y)
//{
//	spawnPos = position;
//}
//
//Enemy::~Enemy()
//{
//	if (receiveDmg != nullptr)
//		receiveDmg->pendingToDelete = true;
//	if (afflictDmg != nullptr)
//		afflictDmg->pendingToDelete = true;
//}
//
//const Collider* Enemy::GetCollider() const
//{
//	return receiveDmg;
//}
//
//void Enemy::Update()
//{
//	if (currentAnim != nullptr)
//		currentAnim->Update();
//
//	if (receiveDmg != nullptr)
//		receiveDmg->SetPos(position.x, position.y);
//}
//
//void Enemy::Draw()
//{
//	if (currentAnim != nullptr)
//		app->render->DrawTexture(texture, position.x - 45, position.y - 40, isFlipped, &currentAnimation->GetCurrentFrame());
//}
//
//void Enemy::OnCollision(Collider* collider)
//{
//	
//}
//
//void Enemy::SetToDelete()
//{
//	pendingToDelete = true;
//	if (receiveDmg != nullptr)
//		receiveDmg->pendingToDelete = true;
//	if (afflictDmg != nullptr)
//		afflictDmg->pendingToDelete = true;
//}