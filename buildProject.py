import os
import yaml
import argparse
import scripts.pro as proj_str

def createDir(dir):
    create_flag = True
    if os.path.exists(dir):
        if os.path.isdir(dir):
            create_flag = False
    if create_flag:
        os.mkdir(dir)

chip_ASMs = {'cl':'startup_stm32f10x_cl.s',
            'hd': 'startup_stm32f10x_hd.s',
            'hd_vl': 'startup_stm32f10x_hd_vl.s',
            'ld':'startup_stm32f10x_ld.s',
            'ld_vl':'startup_stm32f10x_ld_vl.s',
            'md': 'startup_stm32f10x_md.s',
            'md_vl' : 'startup_stm32f10x_md_vl.s',
            'xl' : 'startup_stm32f10x_xl.s'}
chip_defines = {
    'cl':'',
    'hd': 'STM32F10X_HD',
    'hd_vl': '',
    'ld':'',
    'ld_vl':'',
    'md': '',
    'md_vl' : '',
    'xl' : ''
}

parse = argparse.ArgumentParser()
parse.add_argument('--cfgFile',type=str,default='./projCfg.yaml')

args = parse.parse_args()

rf = open(file=args.cfgFile, mode='r', encoding='utf-8')
crf = rf.read()
rf.close()
yaml_data = yaml.load(stream=crf, Loader=yaml.FullLoader)
print(yaml_data)

buildDirName = './projects'
projDirName = buildDirName + '/' + yaml_data['name']
createDir(buildDirName)
createDir(projDirName)

with open(projDirName + '/main.cpp','w') as main_cpp:
    main_cpp.write(proj_str.get_main_str())

include_dirs = ['../../HAL','../../CMSIS','./']
include_dirs = proj_str.include_dir_path2xml(include_dirs)

groups = []
groups.append(proj_str.source_file2groupXml('StartUp',['../../CMSIS/system_stm32f10x.c','../../StartUpAsm/'+chip_ASMs[yaml_data['chip']]]))
groups.append(proj_str.source_file2groupXml('User',['./main.cpp']))
hal_files = []
for f in os.listdir('./HAL'):
    if f.split('.')[-1] == 'cpp':
        hal_files.append('../../HAL/'+f)
groups.append(proj_str.source_file2groupXml('HAL',hal_files))
groups = proj_str.groups_list2groups_xml(groups)
with open(projDirName+f'/{yaml_data["name"]}.uvprojx','w') as proj:
    proj.write(proj_str.get_pro_str(yaml_data['name'], include_dirs, groups,chip_defines[yaml_data['chip']]))
