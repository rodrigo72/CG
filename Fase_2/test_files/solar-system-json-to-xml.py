import json
import xml.dom.minidom as minidom
from enum import IntEnum
import random

SPHERE_MODEL = 'sphere_1_12_12.3d'
LOWER_RES_SPHERE_MODEL = 'sphere_1_5_5.3d'
RING_MODEL = 'ring_3_4_20.3d'


def get_color_for_temperature(temperature):
    temperature_range = [-225, 5600]
    color_range = [(0, 0, 255), (255, 0, 0)]

    normalized_temperature = (temperature - temperature_range[0]) / (temperature_range[1] - temperature_range[0])

    r = int((1 - normalized_temperature) * color_range[0][0] + normalized_temperature * color_range[1][0])
    g = int((1 - normalized_temperature) * color_range[0][1] + normalized_temperature * color_range[1][1])
    b = int((1 - normalized_temperature) * color_range[0][2] + normalized_temperature * color_range[1][2])

    return r, g, b


def create_window(doc, world):
    window = doc.createElement("window")
    window.setAttribute("width", "512")
    window.setAttribute("height", "512")
    world.appendChild(window)


def create_camera(doc, world):
    camera = doc.createElement("camera")

    position = doc.createElement("position")
    position.setAttribute("x", "200")
    position.setAttribute("y", "200")
    position.setAttribute("z", "200")
    camera.appendChild(position)
    
    lookAt = doc.createElement("lookAt")
    lookAt.setAttribute("x", "0")
    lookAt.setAttribute("y", "0")
    lookAt.setAttribute("z", "0")
    camera.appendChild(lookAt)
    
    up = doc.createElement("up")
    up.setAttribute("x", "0")
    up.setAttribute("y", "1")
    up.setAttribute("z", "0")
    camera.appendChild(up)
    
    projection = doc.createElement("projection")
    projection.setAttribute("fov", "90")
    projection.setAttribute("near", "100")
    projection.setAttribute("far", "7000")
    camera.appendChild(projection)
    
    world.appendChild(camera)


def create_scale_xyz(doc, x, y, z):
    scale = doc.createElement('scale')
    scale.setAttribute('x', x)
    scale.setAttribute('y', y)
    scale.setAttribute('z', z)
    return scale


def create_translate_xyz(doc, x, y, z):
    translate = doc.createElement('translate')
    translate.setAttribute('x', x)
    translate.setAttribute('y', y)
    translate.setAttribute('z', z)
    return translate


def create_scale(doc, obj):
    r = str(round((obj['diameter'] / 2.0) / 1000.0)) # tecnicamente devia ser a dividir por 1000000 para ficar à
                                                     # escala porque a distância entre os planetas e o sol está em 10^6
    return create_scale_xyz(doc, r, r, r)


def create_scale_satellite(doc, obj):
    r = str(round(obj['radius'] / 1000.0))
    return create_scale_xyz(doc, r, r, r)


def create_translate(doc, obj):
    distance_from_sun = str(round(obj['distanceFromSun']))
    return create_translate_xyz(doc, distance_from_sun, '0', '0')


def create_translate_satellite(doc, obj, satellite):
    planet_radius = round((obj['diameter'] / 2.0) / 1000.0)
    lower_bound = planet_radius + round(satellite['radius'])
    upper_bound = int(planet_radius * 2) + round(satellite['radius'])
    distance_from_planet = str(random.randint(lower_bound, upper_bound))
    return create_translate_xyz(doc, distance_from_planet, '0', '0')
    

def create_rotate(doc, max_x, y, max_z, max_angle):
    rotate = doc.createElement('rotate')
    rotate_x = str(round(random.uniform(0, max_x)))
    rotate_y = str(y)
    rotate_z = str(round(random.uniform(0, max_z)))
    angle = str(round(random.uniform(0, max_angle)))
    rotate.setAttribute('x', rotate_x)
    rotate.setAttribute('y', rotate_y)
    rotate.setAttribute('z', rotate_z)
    rotate.setAttribute('angle', angle)
    return rotate


def get_satellites(planet_id, satellites):
    satellites_arr = []
    for s in satellites:
        if s['planetId'] == planet_id:
            satellites_arr.append(s)
    return satellites_arr


def create_groups(doc, world, planets, satellites):
    for obj in planets:
        # main group
        main_group = doc.createElement('group')
        transform = doc.createElement('transform')
        # main group - rotate
        rotate = create_rotate(doc, 0.2, 1, 0.2, 360)
        transform.appendChild(rotate)
         # main group - translate
        translate = create_translate(doc, obj)
        transform.appendChild(translate)
        main_group.appendChild(transform)  # append transform to main group
        
         # planet group
        planet_group = doc.createElement('group')
        
        comment = doc.createComment(obj['name'])
        planet_group.appendChild(comment)
        
        color = doc.createElement('color')
        r, g, b = get_color_for_temperature(obj['meanTemperature'])
        color.setAttribute('r', str(r))
        color.setAttribute('g', str(g))
        color.setAttribute('b', str(b))
        planet_group.appendChild(color)
        
        animation = doc.createElement('animation')
        orbital_velocity = str(round(obj['orbitalVelocity']))
        rotation_period = str(round(obj['rotationPeriod']))
        animation.setAttribute('orbitalVelocity', orbital_velocity)
        animation.setAttribute('rotationPeriod', rotation_period)
        planet_group.appendChild(animation)
        
        transform = doc.createElement('transform')
         # planet group - scale
        scale = create_scale(doc, obj)
        transform.appendChild(scale)
        planet_group.appendChild(transform)
        
        planet_models = create_models(doc, SPHERE_MODEL)
        
        if (obj['hasRingSystem']):
            planet_models = create_models(doc, [SPHERE_MODEL, RING_MODEL])
        else:
            planet_models = create_models(doc, [SPHERE_MODEL])
            
        planet_group.appendChild(planet_models)    
        main_group.appendChild(planet_group) # append planet_group to main_group
        
        satellites_arr = get_satellites(obj['id'], satellites)
        
        max_satellites = 3
        counter = 0
        for s in satellites_arr:
            if counter > max_satellites:
                break
            counter += 1
            
            s_group = doc.createElement('group')
            
            comment = doc.createComment(s['name'])
            s_group.appendChild(comment)
            
            transform = doc.createElement('transform')
            
            # rotate
            rotate = create_rotate(doc, 0.2, 1, 0.2, 360)
            transform.appendChild(rotate)
            
            # translate
            translate = create_translate_satellite(doc, obj, s)
            transform.appendChild(translate)
            
            #scale
            scale = create_scale_satellite(doc, s)
            transform.appendChild(scale)
            s_group.appendChild(transform)
            
            s_models = create_models(doc, [LOWER_RES_SPHERE_MODEL])
            s_group.appendChild(s_models)
            
            main_group.appendChild(s_group)
            
        world.appendChild(main_group)
    
    
def create_models(doc, model_files):
    models = doc.createElement('models')
    for model_file in model_files:
        model = doc.createElement('model')
        model.setAttribute('file', model_file)
        models.appendChild(model)
    return models


def main():
    # o sol não está completamente à escala, o diametro devia ser 1391400 e não 1391400
    with open('planets_and_sun.json', 'r') as file:
        planets = json.load(file)
        
    with open('satellites.json', 'r') as file:
        satellites = json.load(file)
                
    doc = minidom.Document()
    world = doc.createElement("world")
    doc.appendChild(world)
    
    create_window(doc, world)
    create_camera(doc, world)
    create_groups(doc, world, planets, satellites)
    
    with open("test_2_6.xml", "w", encoding="utf-8") as xml_file:
        xml_file.write(doc.toprettyxml(indent="\t").split('\n', 1)[1])


if __name__ == '__main__':
    main()
