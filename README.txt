La tematica principal del proyecto se basa en un Sistema Planetario. Aun asi, me he  
centrado mas en las funcionalidades que no hacer el sistema realista. 

El sistema planetario tiene:
- Planeta tierra con multitexturas con rotacion y iluminaci�n completa con diffuse, specular i ambient. Shader independiente.
- Luna que rota como centro la tierra con iluminaci�n completa y transparencia. Mismo shader que el del planeta azul. 
- Sol en el centro sin iluminaci�n ya que crea el punto central de luz.
- SkyBox con la textura de milkyway proporcionada sin iluminaci�n. Tanto el skybox y el sol tienen el mismo shader.
- Planeta mas peque�o que rota con el centro el sol e iluminaci�n completa sin multitextura pero con transparencia. 
Mismo shader que la luna.

Nota: El clipping lo he habilitado. Apretando la L se desbloquea el mouse que se mantenga en el centro.