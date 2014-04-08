engiX
=====

Experimental Game Engine based on DirectX11 and the high performance SIMD DirectXMath library

Features:
- Organized in 3 layers: App and View layer which are platform dependant, Logic layer which is platform agnostic.
- Leverages the DirectX DXUT11 framework to simplify the App and View layer implementation.
- Uses the high-perofmrance and optimized SIMD-friendly C++ math library DirectXMath for common linear algebra and graphics math operations.
- 3D Scene management (under implementation)
- Keyboard Input handling.
- Event based system using a centralized message pump.
- The game actor architecture is component based, and an actor is defined as the aggregation of 1 or more component. Example: PhysicsComponent, TransformComponent, D3dMeshComponent, AiComponent, etc.. (under implementation).

3rdParty Libs:
- DXUT framework
- TinyXml2
