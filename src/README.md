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

# pre visitor storage
krys@krys-desktop:~/prj/deca_lib/src$ python deca_adf_wasm_test.py 
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 4.387384414672852 = 0.0006453990936279297 + 0.08149051666259766 + 4.305248498916626
../test/rtpc/global_gui.blo, value = 1, Time: 0.034533023834228516 = 4.315376281738281e-05 + 0.0005993843078613281 + 0.033890485763549805
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.010692834854125977 = 4.410743713378906e-05 + 0.00025391578674316406 + 0.010394811630249023
../test/adf/animal_population_0, value = 1, Time: 0.212113618850708 = 2.86102294921875e-05 + 0.009018898010253906 + 0.20306611061096191
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
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0007164478302001953 = 3.4809112548828125e-05 + 0.0001633167266845703 + 0.0005183219909667969
../test/adf/savegame, value = 1, Time: 2.7818617820739746 = 0.0006597042083740234 + 0.15050363540649414 + 2.6306984424591064

# post visitor storage
krys@krys-desktop:~/prj/deca_lib/src$ python test_wasm_lib.py 
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 4.408195972442627 = 0.0006291866302490234 + 0.08305668830871582 + 4.324510097503662
../test/rtpc/global_gui.blo, value = 1, Time: 0.075714111328125 = 3.743171691894531e-05 + 0.0006239414215087891 + 0.07505273818969727
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.012140512466430664 = 3.528594970703125e-05 + 0.000244140625 + 0.011861085891723633
../test/adf/animal_population_0, value = 1, Time: 0.19366168975830078 = 2.5033950805664062e-05 + 0.008695363998413086 + 0.18494129180908203
hash_register(00000000046acfce, 5264680, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5264722, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5264746, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5264764, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5264804, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5264870, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5264946, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5264997, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265073, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265113, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265189, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0007047653198242188 = 4.506111145019531e-05 + 0.00016427040100097656 + 0.0004954338073730469
../test/adf/savegame, value = 1, Time: 2.7698512077331543 = 0.0013284683227539062 + 0.15267133712768555 + 2.615851402282715

```

# new file access
## reference
```
krys@krys-desktop:~/prj/deca_lib/src$ python test_wasm_lib.py 
file_open(stdout,rwb)
file_size(0)
file_open(stderr,rwb)
file_size(1)
file_open(stdin,rwb)
file_size(2)
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 4.688530921936035 = 0.0006258487701416016 + 0.08438754081726074 + 4.603517532348633
../test/rtpc/global_gui.blo, value = 1, Time: 0.0775299072265625 = 3.8623809814453125e-05 + 0.000652313232421875 + 0.07683897018432617
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.011308908462524414 = 4.315376281738281e-05 + 0.00035881996154785156 + 0.01090693473815918
file_open(xml_in,)
file_open(xml_out,)
xml_process(3, 4)
file_close(4)
file_close(3)
file_open(xml_in,)
file_open(xml_out,)
xml_process(5, 6)
file_close(6)
file_close(5)
file_open(xml_in,)
file_open(xml_out,)
xml_process(7, 8)
file_close(8)
file_close(7)
../test/adf/animal_population_0, value = 1, Time: 0.22817420959472656 = 2.0265579223632812e-05 + 0.009349822998046875 + 0.21880412101745605
hash_register(00000000046acfce, 5265360, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5265402, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5265426, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5265444, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5265484, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5265550, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5265626, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5265677, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265753, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265793, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265869, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0007665157318115234 = 3.361701965332031e-05 + 0.00015354156494140625 + 0.0005793571472167969
../test/adf/savegame, value = 1, Time: 2.958386182785034 = 0.0013806819915771484 + 0.17192769050598145 + 2.7850778102874756
krys@krys-desktop:~/prj/deca_lib/src$ python test_wasm_lib.py 
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 4.654101371765137 = 0.0006096363067626953 + 0.08500528335571289 + 4.568486452102661
../test/rtpc/global_gui.blo, value = 1, Time: 0.07743263244628906 = 3.910064697265625e-05 + 0.0006303787231445312 + 0.07676315307617188
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.010978460311889648 = 3.5762786865234375e-05 + 0.0002639293670654297 + 0.010678768157958984
xml_process(3, 4)
xml_process(5, 6)
xml_process(7, 8)
../test/adf/animal_population_0, value = 1, Time: 0.21946120262145996 = 1.621246337890625e-05 + 0.009126663208007812 + 0.21031832695007324
hash_register(00000000046acfce, 5265360, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5265402, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5265426, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5265444, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5265484, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5265550, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5265626, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5265677, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265753, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265793, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265869, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0006728172302246094 = 3.3855438232421875e-05 + 0.00014734268188476562 + 0.0004916191101074219
../test/adf/savegame, value = 1, Time: 2.917729616165161 = 0.0013778209686279297 + 0.15984177589416504 + 2.756510019302368
```
### refactor in prep for new file access
```
krys@krys-desktop:~/prj/deca_lib/src$ python test_wasm_lib.py 
wasm memory size = 268435456
../test/rtpc/global_aesir.blo, value = 1, Time: 4.702573776245117 = 0.0006198883056640625 + 0.08656835556030273 + 4.61538553237915
../test/rtpc/global_gui.blo, value = 1, Time: 0.08048343658447266 = 4.0531158447265625e-05 + 0.0006372928619384766 + 0.07980561256408691
../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.010854005813598633 = 3.504753112792969e-05 + 0.00024890899658203125 + 0.010570049285888672
xml_process(3, 4)
xml_process(5, 6)
xml_process(7, 8)
../test/adf/animal_population_0, value = 1, Time: 0.1989448070526123 = 1.430511474609375e-05 + 0.009181022644042969 + 0.18974947929382324
hash_register(00000000046acfce, 5265376, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5265418, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5265442, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5265460, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5265500, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5265566, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5265642, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5265693, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265769, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265809, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265885, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0006687641143798828 = 3.314018249511719e-05 + 0.00014925003051757812 + 0.0004863739013671875
../test/adf/savegame, value = 1, Time: 2.9384825229644775 = 0.001371145248413086 + 0.15947484970092773 + 2.7776365280151367
```

### more prep
```
wasm memory size = 268435456
Mem: ../test/rtpc/global_aesir.blo, value = 1, Time: 4.409419775009155 = 0.0006041526794433594 + 0.08687257766723633 + 4.321943044662476
Mem: ../test/rtpc/global_gui.blo, value = 1, Time: 0.08058619499206543 = 3.814697265625e-05 + 0.0006194114685058594 + 0.07992863655090332
Mem: ../test/rtpc/abandoned_foa_facility_acoustics.blo, value = 1, Time: 0.01092982292175293 = 3.981590270996094e-05 + 0.00036144256591796875 + 0.010528564453125
xml_process(3, 4)
xml_process(5, 6)
xml_process(7, 8)
../test/adf/animal_population_0, value = 1, Time: 0.19135689735412598 = 2.2172927856445312e-05 + 0.009649515151977539 + 0.181685209274292
hash_register(00000000046acfce, 5265392, 33) == b'textures/dummies/dummy_white.ddsc'
hash_register(000000001c3f3cf7, 5265434, 15) == b'Pistol_Sa_22_01'
hash_register(00000000308f2f7c, 5265458, 9) == b'GeneralR2'
hash_register(00000000764ca8d0, 5265476, 31) == b'textures/dummies/dummy_nrm.ddsc'
hash_register(00000000812013bb, 5265516, 57) == b'models/hp_weapons/pistol_sa_22_01/pistol_sa_22_01_a.meshc'
hash_register(0000000087db5283, 5265582, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_dif.ddsc'
hash_register(00000000b0bd45f7, 5265658, 42) == b'textures/dummies/dummy_grey_alpha_dif.ddsc'
hash_register(00000000bb565b5a, 5265709, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_nrm.ddsc'
hash_register(00000000ceb44da8, 5265785, 31) == b'textures/dummies/dummy_mpm.ddsc'
hash_register(00000000d2fd9902, 5265825, 67) == b'models/hp_weapons/pistol_sa_22_01/textures/handgun_22sa_01_mpm.ddsc'
hash_register(00000000deadbeef, 5265901, 0) == b''
../test/adf/pistol_sa_22_01_a.modelc, value = 1, Time: 0.0006766319274902344 = 3.218650817871094e-05 + 0.0001499652862548828 + 0.0004944801330566406
../test/adf/savegame, value = 1, Time: 2.7572476863861084 = 0.0013082027435302734 + 0.1568291187286377 + 2.5991103649139404
```