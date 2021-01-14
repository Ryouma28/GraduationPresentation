//=================================================================================
//�^��`
//=================================================================================
struct VS_OUT
{
    float4 Position	: POSITION;
    float4 Color	: COLOR0;
    float2 Texture : TEXCOORD0;
};

//=================================================================================
//�O���[�o��
//=================================================================================
float4x4 WVP; //���[���h�s��
float4x4 WIT;  //���[���h�s��̋t�s��̓]�u�s��
float4 LightDir;  //�����C�g�̕����x�N�g��
float4 LightIntensity; //���ˌ��̋��x

float4 Diffuse = {1.0f, 1.0f, 1.0f, 1.0f}; //�g�U���˗�
float4 Ambient = {1.0f, 1.0f, 1.0f, 1.0f};	// ����

texture texDecal; //���b�V���̃e�N�X�`��
texture NormalMap;  // �m�[�}���}�b�s���O

sampler Sampler = sampler_state //�T���v���[
{
    Texture = <texDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler NormalSampler = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

//=================================================================================
//�o�[�e�b�N�X�E�V�F�[�_�[
//=================================================================================
VS_OUT VS(float4 Position : POSITION, float3 Normal : NORMAL, float2 Texture : TEXCOORD)
{
	VS_OUT Out = (VS_OUT)0;   
	
	Out.Position=mul(Position,WVP);
	
	float3 WNormal=normalize(mul(Normal,(float3x3)WIT));

    // float4 col = {1.0f, 1.0f, 1.0f, 1.0f};

	// col.xyz = Ambient.xyz * LightIntensity.xyz * Diffuse.xyz * dot(WNormal,-LightDir.xyz);
    // col.w = Diffuse.w;

    // float4 BaseCol = {1.0f, 1.0f, 1.0f, 0.0f};
    // // Out.Color = col * BaseCol;
    Out.Color = LightIntensity * Diffuse * max(Ambient, dot(WNormal,-LightDir.xyz));
    Out.Texture = Texture;
	return Out;
}
//=================================================================================
//�s�N�Z���E�V�F�[�_�[
//=================================================================================
float4 PS(VS_OUT In) : COLOR
{   
    return In.Color * 2 * tex2D(Sampler, In.Texture);
}

//=================================================================================
// �e�N�j�b�N
//=================================================================================
technique tecLambert
{
    pass Lambert
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}