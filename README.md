The theme of this project, as you can see by the title, is about a Star system. Its not focused on adding planets reallistically but more into adding OpenGL features, like rotation, transparency, etc.

It has de following features:
- Earth like planet with multitextures and rotation around the star. It has also diffuse, specular and ambient lighting. Independent shader.
- Moon like planet that rotates around the earth. Contains same lighting as earth, also including transparency. Sharing shader with the blue planet.
- Star in the center. It has no lighting effect as it has to simulate the light in the star system. 
- Skybox with the milkyway texture, without lighting. Both the skybox and the star shares the same shader. 
- Blue planet that rotates around the sun. It has de same features as the moon, full lighting and a bit of transparency.  

Controls:
- W: Forward
- S: Backwards
- A: Left
- D: Right
- F: Down
- R: Up
- Z: Increase elocity
- X: Decrease  velocity
- V: Increase rotation
- B: Decrease rotacion
- L: Activate/Deactivate Mouse Lock
- ESPACIO: Center mouse in center
