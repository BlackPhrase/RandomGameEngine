
#ifndef BULLET_SPAWNER_H
#define BULLET_SPAWNER_H

class CBullet;

class IBulletSpawner
{
public:
	virtual void SpawnBullet( CBullet *pBullet ) = 0;
	virtual double EngineTime() = 0;
};

#endif // BULLET_SPAWNER_H