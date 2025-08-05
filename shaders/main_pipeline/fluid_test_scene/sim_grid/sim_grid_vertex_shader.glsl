#version 460 core

layout (location = 10) in int grid_cell_num_particles_count;
layout (location = 11) in vec3 grid_cell_shape_ver_pos;


uniform mat4 view;
uniform mat4 projection;
uniform vec3 bounding_pos;
uniform vec3 bounding_dims;
uniform vec3 cell_dims;
uniform int n_cells_x;
uniform int n_cells_y;
uniform int n_cells_z;

out flat int GridCellNumParticlesCount;

vec3 cell_idx_to_grid_center_pos(int index){
    vec3 pos = vec3(0,0,0);
    pos.x = int(mod(index, n_cells_x));
    pos.y = int(mod((index - pos.x) / n_cells_x, n_cells_y));
    pos.z = int(((index - pos.x) / n_cells_x - pos.y) / n_cells_y);

    pos.x *= cell_dims.x;
    pos.y *= cell_dims.y;
    pos.z *= cell_dims.z;
    pos += vec3(cell_dims.x / 2.0, cell_dims.y / 2.0, cell_dims.z / 2.0);

    pos += bounding_pos - bounding_dims / 2.0;

    return pos;
}

void main(){

vec3 grid_cell_center_pos = cell_idx_to_grid_center_pos(gl_InstanceID);
gl_Position = projection * view * vec4(grid_cell_shape_ver_pos + grid_cell_center_pos, 1.0);

GridCellNumParticlesCount = grid_cell_num_particles_count;

}