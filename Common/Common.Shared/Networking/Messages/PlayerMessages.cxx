#include "PlayerMessages.hxx"

PlayerJoin::PlayerJoin()
{ }

const std::wstring PlayerJoin::GetName() const
{
	return player_name;
}

void PlayerJoin::SetName(const std::wstring& name)
{
	player_name = name;
}

const size_t PlayerJoin::GetSender() const
{
	return sender;
}

void PlayerJoin::SetSender(size_t id)
{
	sender = id;
}

PlayerQuit::PlayerQuit()
{ }

const size_t PlayerQuit::GetSender() const
{
	return sender;
}

void PlayerQuit::SetSender(size_t id)
{
	sender = id;
}

PlayerSpawn::PlayerSpawn()
{ }

const uint32_t PlayerSpawn::GetModelHash() const
{
	return model_hash;
}

void PlayerSpawn::GetPosition(Vector3& _position) const
{
	_position = position;
}

void PlayerSpawn::GetRotation(Vector3& _rotation) const
{
	_rotation = rotation;
}

const size_t PlayerSpawn::GetSender() const
{
	return sender;
}

void PlayerSpawn::SetModelHash(uint32_t hash)
{
	model_hash = hash;
}

void PlayerSpawn::SetPosition(const Vector3& _position)
{
	position = _position;
}

void PlayerSpawn::SetRotation(const Vector3& _rotation)
{
	rotation = _rotation;
}

void PlayerSpawn::SetSender(size_t id)
{
	sender = id;
}

PlayerDespawn::PlayerDespawn()
{ }

const size_t PlayerDespawn::GetSender() const
{
	return sender;
}

void PlayerDespawn::SetSender(size_t id)
{
	sender = id;
}
