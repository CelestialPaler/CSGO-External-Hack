#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT numElements)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to load shader.";
			msg.szTarget = Util::StringManipulation::WstringToString(shaderpath);
			logger->Write(msg);
		}
		return false;
	}

	hr = device->CreateVertexShader(this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create vertex shader.";
			msg.szTarget = Util::StringManipulation::WstringToString(shaderpath);
			logger->Write(msg);
		}
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numElements, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create input layout.";
			logger->Write(msg);
		}
		return false;
	}

	return true;
}

ID3D11VertexShader * VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob * VertexShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

ID3D11InputLayout * VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to load shader.";
			msg.szTarget = Util::StringManipulation::WstringToString(shaderpath);
			logger->Write(msg);
		}
		return false;
	}

	hr = device->CreatePixelShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create pixel shader.";
			msg.szTarget = Util::StringManipulation::WstringToString(shaderpath);
			logger->Write(msg);
		}
		return false;
	}

	return true;
}

ID3D11PixelShader * PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob * PixelShader::GetBuffer()
{
	return this->shader_buffer.Get();
}