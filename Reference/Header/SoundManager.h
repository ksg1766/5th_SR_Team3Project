#pragma once
#include "fmod.h"
#include "fmod.hpp"
#include <io.h>

#include "Engine_Define.h"
#include "Base.h"


BEGIN(Engine)

class ENGINE_DLL CSoundManager : public CBase
{
	DECLARE_SINGLETON(CSoundManager);
private:
	explicit CSoundManager();
	virtual ~CSoundManager();

public:
	void Initialize();
	void Release();

public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free() override;
};

END