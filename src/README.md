emcc dxgi.cpp -o dxgi.wasm -O3 -s EXPORT_ALL=0 -s EXPORTED_FUNCTIONS="['_sum']"
emcc dxgi.cpp -o dxgi.wasm -O3 -v -s INITIAL_MEMORY=256MB -s MAXIMUM_MEMORY=1GB -s EXPORT_ALL=0 -s EXPORTED_FUNCTIONS="['_process_image', '_alloc_bin', '_alloc_bout']"

// -s ALLOW_MEMORY_GROWTH 

DataViewWidget:vnode_2click_selected: ft:avtx h:c5a0a057 v:b'climate/hp_germany_farmland_fall/zone_0/diffuse_textures_mid_0_0.ddsc'

C Time 0.012518644332885742
C Time 0.012247323989868164
C Time 0.012452840805053711
C Time 0.012439966201782227
C Time 0.012502193450927734

Python Time 0.024748802185058594
Python Time 0.02440786361694336
Python Time 0.024622678756713867
Python Time 0.02463388442993164
Python Time 0.024763107299804688

# PyPy
WASM Time 0.3640730381011963 =  0.15358543395996094 + 0.024332523345947266 + 0.18615508079528809
WASM Time 0.29267144203186035 =  0.1460726261138916 + 0.018781185150146484 + 0.12781763076782227
WASM Time 0.2948896884918213 =  0.14873766899108887 + 0.01877737045288086 + 0.12737464904785156
WASM Time 0.28647565841674805 =  0.14014005661010742 + 0.01883864402770996 + 0.12749695777893066
WASM Time 0.2877676486968994 =  0.1412062644958496 + 0.01900458335876465 + 0.12755680084228516

# LLVM
WASM Time 0.2168431282043457 =  0.19345641136169434 + 0.014397859573364258 + 0.00898885726928711
WASM Time 0.18523454666137695 =  0.1735365390777588 + 0.009010791778564453 + 0.002687215805053711
WASM Time 0.18828964233398438 =  0.1733231544494629 + 0.009198188781738281 + 0.005768299102783203
WASM Time 0.19315028190612793 =  0.1746227741241455 + 0.009212017059326172 + 0.00931549072265625
WASM Time 0.19151973724365234 =  0.17299556732177734 + 0.009221553802490234 + 0.009302616119384766


```
Connected to pydev debugger (build 202.6397.106)
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 6.753524303436279 = 0.0006747245788574219 + 0.0810856819152832 + 6.671763896942139
../test/rtpc/global_gui.blo, value = 1, Time: 0.051778316497802734 = 5.3882598876953125e-05 + 0.0007443428039550781 + 0.0509800910949707
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.015992403030395508 = 4.1484832763671875e-05 + 0.00024819374084472656 + 0.01570272445678711
../test/adf/animal_population_0, value = 1, Time: 0.36406660079956055 = 2.5987625122070312e-05 + 0.008669137954711914 + 0.35537147521972656
hash_register(00000000046acfce, 5264728, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5264770, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5264794, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5264812, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5264852, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5264918, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5264994, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5265045, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265121, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265161, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265237, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0010726451873779297 = 4.649162292480469e-05 + 0.00014472007751464844 + 0.0008814334869384766
../test/adf/savegame, value = 1, Time: 5.194193363189697 = 0.0005421638488769531 + 0.15038084983825684 + 5.0432703495025635

/home/krys/.pyenv/versions/venv37/bin/python /snap/clion/123/plugins/python-ce/helpers/pydev/pydevd.py --multiproc --qt-support=auto --client 127.0.0.1 --port 46273 --file /home/krys/prj/deca_lib/src/deca_adf_wasm_test.py
pydev debugger: process 417189 is connecting

Connected to pydev debugger (build 202.6397.106)
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 6.569980621337891 = 0.001071929931640625 + 0.07882428169250488 + 6.490084409713745
../test/rtpc/global_gui.blo, value = 1, Time: 0.04758644104003906 = 5.984306335449219e-05 + 0.0006263256072998047 + 0.046900272369384766
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.01865696907043457 = 4.0531158447265625e-05 + 0.0002536773681640625 + 0.018362760543823242
../test/adf/animal_population_0, value = 1, Time: 0.36504244804382324 = 3.218650817871094e-05 + 0.008705377578735352 + 0.3563048839569092
hash_register(00000000046acfce, 5264728, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5264770, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5264794, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5264812, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5264852, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5264918, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5264994, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5265045, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265121, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265161, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265237, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0010685920715332031 = 5.459785461425781e-05 + 0.00015473365783691406 + 0.0008592605590820312
../test/adf/savegame, value = 1, Time: 5.219261884689331 = 0.0006237030029296875 + 0.148345947265625 + 5.070292234420776
```