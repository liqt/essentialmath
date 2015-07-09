//===============================================================================
// @ IvConstantTableD3D11.h
// 
// Replacement for D3D9 constant table
// ------------------------------------------------------------------------------
// Copyright (C) 2014   James M. Van Verth & Lars M. Bishop
//
// Change history:
//
// Usage notes
//===============================================================================

#ifndef __IvConstantTableD3D11__h__
#define __IvConstantTableD3D11__h__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------
#include <d3d11.h>
#include <map>

#include "IvUniform.h"

//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

typedef int IvConstantHandle;

struct IvConstantDesc
{
	IvUniformType mType;
	union
	{
		struct
		{
			void*         mOffset;
			unsigned int  mCount;
		};
		struct
		{
			int           mTextureSlot;
			int           mSamplerSlot;
		};
	};
};

class IvConstantTableD3D11
{
public:
	static IvConstantTableD3D11* Create(ID3D11Device* device, ID3DBlob* code);

	void AddRef() { ++mRefCount; }
	void Release() { Destroy(this); }

	bool GetConstantDesc(const char* name, IvConstantDesc* constantDesc);
	void MarkDirty() { mDirty = true;  }

	bool MakeActiveVS(ID3D11DeviceContext* context);
	bool MakeActivePS(ID3D11DeviceContext* context);

private:
	static void Destroy(IvConstantTableD3D11* table);

	// constructor/destructor
	IvConstantTableD3D11() : mRefCount(1), mBuffer(NULL), mBacking(NULL), 
		                    mDirty(false) {}
	~IvConstantTableD3D11()
	{
		if (mBuffer)
		{
			mBuffer->Release();
			mBuffer = NULL;
		}
		delete mBacking;
	}

private:
	// copy operations
	IvConstantTableD3D11(const IvConstantTableD3D11& other);
	IvConstantTableD3D11& operator=(const IvConstantTableD3D11& other);

	int									  mRefCount;
	std::map<std::string, IvConstantDesc> mConstants;
	ID3D11Buffer*						  mBuffer;
	char*                                 mBacking;
	size_t								  mBackingSize;
	bool								  mDirty;
};


//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Externs --------------------------------------------------------------------
//-------------------------------------------------------------------------------

#endif