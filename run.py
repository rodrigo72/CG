import sys, shutil, os, time, subprocess
from typing import Dict, List


CONFIG_TYPE = 'Debug'
ENGINE_PATH = './Fase_{}/build/{}/engine.exe'
GENERATOR_PATH = './Fase_{}/build/{}/generator.exe'
TEST_FILES_FOLDER_PATH = './Fase_{}/test_files'

test_info = {
    1: {
        1: ['cone 1 2 4 3 cone_1_2_4_3.3d'],
        2: ['cone 1 2 4 3 cone_1_2_4_3.3d'],
        3: ['sphere 1 10 10 sphere_1_10_10.3d'],
        4: ['box 2 3 box_2_3.3d'],
        5: ['plane 2 3 plane_2_3.3d', 'sphere 1 10 10 sphere_1_10_10.3d']
    },
    2: {
        1: ['box 2 3 box_2_3.3d'],
        2: ['box 2 3 box_2_3.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d', 'sphere 1 8 8 sphere_1_8_8.3d'],
        3: ['sphere 1 8 8 sphere_1_8_8.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d'],
        4: ['box 2 3 box_2_3.3d'],
        5: ['sphere 1 20 20 sphere_1_20_20.3d', 'ring 10 15 20 ring_10_15_20.3d'],
        6: ['sphere 1 12 12 sphere_1_12_12.3d', 'ring 3 4 20 ring_3_4_20.3d', 'sphere 1 5 5 sphere_1_5_5.3d']
    },
    3: {
        1: ['patch 10 ../patches/teapot.patch bezier_10.3d'],
        2: ['patch 10 ../patches/teapot.patch bezier_10.3d'],
        3: ['patch 10 ../patches/teapot.patch bezier_10.3d'], # teapot without new rotate or translate type
        4: ['sphere 1 8 8 sphere_1_8_8.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d'],  # fase 2 test (to confirm that normal rotate and translate still works)
        5: ['patch 10 ../patches/teapot.patch bezier_10.3d', 'sphere 1 12 12 sphere_1_12_12.3d', 'ring 3 4 20 ring_3_4_20.3d', 'sphere 1 5 5 sphere_1_5_5.3d']
    },
    4: {
         0: ['patch 10 ../patches/teapot.patch bezier_10.3d', 'box 2 3 box_2_3.3d', 'plane 2 3 plane_2_3.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d', 'sphere 1 10 10 sphere_2_10_10.3d', 'ring 3 4 20 ring_3_4_20.3d'], # teste da fase anterior
         1: ['box 2 3 box_2_3.3d'],
         2: ['box 2 3 box_2_3.3d'],
         3: ['plane 2 3 plane_2_3.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d', 'sphere 1 8 8 sphere_1_8_8.3d', 'patch 10 ../patches/teapot.patch bezier_10.3d', 'box 2 3 box_2_3.3d'],
         4: ['plane 2 3 plane_2_3.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d', 'sphere 1 8 8 sphere_1_8_8.3d', 'patch 10 ../patches/teapot.patch bezier_10.3d', 'box 2 3 box_2_3.3d'],
         5: ['plane 2 3 plane_2_3.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d', 'sphere 1 8 8 sphere_1_8_8.3d', 'patch 10 ../patches/teapot.patch bezier_10.3d', 'box 2 3 box_2_3.3d'],
         6: ['plane 2 3 plane_2_3.3d', 'cone 1 2 4 3 cone_1_2_4_3.3d', 'sphere 1 8 8 sphere_1_8_8.3d', 'patch 10 ../patches/teapot.patch bezier_10.3d', 'box 2 3 box_2_3.3d'],
         7: ['patch 10 ../patches/teapot.patch bezier_10.3d',  'sphere 1 21 21 sphere_1_21_21.3d', 'ring 3 4 20 ring_3_4_20.3d', 'sphere 1 5 5 sphere_1_5_5.3d'],
         8: ['sphere 1 20 20 sphere_1_20_20.3d', 'cone 1 2 10 7 cone_2_3_10_7.3d']
    }
}


def test(fase: int, test_num: int, paths: Dict[str, str]):    
    shutil.copyfile(paths['original_engine_path'], paths['destination_engine_path'])
    shutil.copyfile(paths['original_generator_path'], paths['destination_generator_path'])
    
    for model in test_info[fase][test_num]:
        args = model.split(' ');
        if not os.path.exists('./' + args[-1]):
            subprocess.run([paths['destination_generator_path']] + args, cwd=paths['test_files_path'])
            
    subprocess.run([
        paths['destination_engine_path'],
        'test_' + str(fase) + '_' + str(test_num) + '.xml'
    ], cwd=paths['test_files_path'])
    
    os.remove(paths['destination_engine_path'])
    os.remove(paths['destination_generator_path'])
    
    
def generate(paths: Dict[str, str], args: List[str]):
    subprocess.run([paths['destination_generator_path']] + args, cwd=paths['test_files_path'])
    
    
def print_usage(name: str) -> str:
    print('Usage: ',
          f'\n\t{name} test <fase> <test_number>',
          f'\n\t{name} test <fase> all',
          f'\n\t{name} generate <fase> <figure> <args>',
          f'\n\t{name} engine <fase> <model>.3d')


def main(argv):
    if len(argv) < 3:
        print_usage(argv[0])
        sys.exit(1)
        
    fase = int(argv[2])
    
    engine_path = ENGINE_PATH.format(fase, CONFIG_TYPE)
    if not os.path.exists(engine_path):
        print(f'{engine_path} does not exist.')
        
    generator_path = GENERATOR_PATH.format(fase, CONFIG_TYPE)
    if not os.path.exists(generator_path):
        print(f'{generator_path} does not exist.')
        
    test_files_folder_path = TEST_FILES_FOLDER_PATH.format(fase, CONFIG_TYPE)
    if not os.path.exists(test_files_folder_path):
        print(f'{test_files_folder_path} does not exist.')
        
    paths = {
        'original_engine_path': engine_path,
        'original_generator_path': generator_path,
        'test_files_path': test_files_folder_path,
        'destination_engine_path': os.path.join(test_files_folder_path, 'engine.exe'),
        'destination_generator_path': os.path.join(test_files_folder_path, 'generator.exe')
    }
            
    if argv[1] == 'test':
        if argv[3] == 'all':
            pass
        else:
            test(fase, int(argv[3]), paths)
    elif argv[1] == 'generate':
        generate(paths, argv[2:])
    else:
        print('Invalid')


if __name__ == '__main__':
    main(sys.argv)
