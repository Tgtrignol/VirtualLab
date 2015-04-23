//uniform float time;
uniform sampler2D texture;

void main()
{
	gl_FragColor = vec4((texture2D(texture, gl_TexCoord[0].st).stp), 1.0);
}