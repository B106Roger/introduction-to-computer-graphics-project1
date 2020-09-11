#version 410 core

in vec2 vUv;
in float mynoise;

float random( vec3 scale, float seed ){
  return fract( sin( dot( gl_FragCoord.xyz + seed, scale ) ) * 43758.5453 + seed ) ;
}

uniform sampler2D tExplosion;

void main() {

  // get a random offset
  float r = .01 * random( vec3( 12.9898, 78.233, 151.7182 ), 0.0 );
  // lookup vertically in the texture, using noise and offset
  // to get the right RGB colour
  vec2 tPos = vec2( 0, 15.f * mynoise + r );
  vec4 color = texture2D( tExplosion, tPos );

  gl_FragColor = vec4( color.rgb, 1.0 );

  // compose the colour using the UV coordinate
  // and modulate it with the noise like ambient occlusion
//   vec3 color = vec3( vUv * ( 1. - 2. * mynoise ), 0.0 );
//   gl_FragColor = vec4( color.rgb, 1.0 );

}
