//
// Created by ZZK on 2024/7/20.
//

#include "d3d12_command_list8_wrap.h"

namespace gfxshim
{
    ID3D12GraphicsCommandList8Wrapper::ID3D12GraphicsCommandList8Wrapper(const IID &riid, IUnknown *object)
    : ID3D12GraphicsCommandList7Wrapper(riid, object)
    {

    }

    ID3D12GraphicsCommandList8Wrapper::~ID3D12GraphicsCommandList8Wrapper() = default;
}