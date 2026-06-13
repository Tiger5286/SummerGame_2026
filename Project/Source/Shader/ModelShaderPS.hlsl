// 頂点シェーダーの出力
struct PSInput
{
    float4 color : COLOR0; // ディフューズカラー
    float4 spec : COLOR1; // スペキュラカラー
    float4 uv : TEXCOORD0; // xy:テクスチャ座標 zw:サブテクスチャ座標
    float3 viewPos : TEXCOORD1; // 座標( ビュー空間 )
    float3 norm : TEXCOORD2; // 法線( ビュー空間 )
    float2 Fog : TEXCOORD5; // フォグパラメータ( x )  高さフォグパラメータ( y )
    float4 svPos : SV_POSITION; // 座標( プロジェクション空間 )
    float3 pos : Position; //単純なワールド座標
    float3 wnorm : Normal; //単純なワールド法線
};

cbuffer CBuffer : register(b4)
{
    float3 cameraPos;
    float dummy;
};

struct PSOutput
{
    float4 col : SV_TARGET0;
    float4 bloom : SV_TARGET1;
};

PSOutput main(PSInput input)
{
    PSOutput output;
    //output.col = float4(1.0, 1.0, 1.0, 1.0);
    //output.bloom = float4(0.0, 0.0, 0.0, 0.0);
    //return output;
    
    //float3 cameraPos = float3(0, 20, -50);
    float3 light = normalize(float3(1, -1, 1));
    const float ambient = 0.2;
    float3 ray = normalize(input.pos.xyz - cameraPos);
    float3 wnorm = normalize(input.wnorm);
    float rim = pow(1.0 - saturate(dot(-ray, input.wnorm)), 8.0);
    float spc = pow(saturate(dot(reflect(light, wnorm), -ray)), 20.0);
    float3 diffuse = input.color.rgb * max(dot(wnorm, -light), ambient);
    float3 rimColor = float3(1.0, 0.8, 0.25);
    output.col = float4(diffuse * rim + spc, input.color.a);
    output.bloom = float4(rim * rimColor, 1);
    return output;
}