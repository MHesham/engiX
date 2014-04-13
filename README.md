engiX
=====

Experimental Game Engine based on DirectX11 and the high performance SIMD DirectXMath library

engiX design is inspired by the sample Game Engine that comes with Game Coding Complete 4 book, https://code.google.com/p/gamecode4/

Features:
- Organized in 3 layers: App and View layers which are platform dependant, Logic layer which is platform agnostic.
- Leverages the DirectX DXUT11 framework to simplify the App and View layer implementation.
- Uses the high-perofmrance and optimized SIMD-friendly C++ math library DirectXMath for common linear algebra and graphics math operations.
- 3D Scene management (under implementation)
- Works with HLSL shaders model 4.
- Keyboard Input handling.
- Event based system using a centralized message pump.
- The game actor architecture is component based, and an actor is defined as the aggregation of 1 or more component. Example: PhysicsComponent, TransformComponent, D3dMeshComponent, AiComponent, etc.. (under implementation).


3rdParty Libs:
- DXUT framework
- TinyXml2

Future Work:
- .NET based 3D Level Editor.
- Implement resource caching for game assets.
- Support lights.
- Support shadows.
- Support Audio and sound effects.
- Add support for XBOX One by implementing XBOX One specific App and View layers.
 
References
Game Coding Complete 4 by Mike McShaffry
Introduction to 3D Game Programming with DirectX 11 by Frank Luna
