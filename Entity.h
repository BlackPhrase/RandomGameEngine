
#ifndef ENTITY_H
#define ENTITY_H

class CEntity
{
public:
	CEntity();
	~CEntity();

	void MarkForDeletion();
	bool IsMarkedForDeletion() const;
	void Reset();

private:
	bool m_bIsMarkedForDeletion;
};

#endif // ENTITY_H
