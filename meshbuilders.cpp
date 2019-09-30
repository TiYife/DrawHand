#include "meshbuilders.h"

unique_ptr<Mesh> MeshBuilders::CreateSphere(Vec3 pos, double r)
{
    unique_ptr<Mesh> mesh(new Mesh());
    int row;
        if (r < 3.f / 20)
            row = 3;
        else
            row = static_cast<int>((log(r * 20 + 0.2) * 1.8f + 1));

        int col = row * 2 + 2;
        if (col < 3) col = 3;
        int ver_count = (row * 2 - 1) * col + 2;
        mesh->positions_.resize(ver_count,Vec3(0,0,0));
        auto angel_step = static_cast<real_t >(M_PI * 2 / col);
        int up_idx = 0;
        //除中心线外
        for (int i = 0; i < row; ++i) {
            if (i == 0) {
                Vec3 vec(0, r, 0);
                mesh->positions_[up_idx++] = Vec3(0, r, 0);
                //注意 up_idx已经加一了
                mesh->positions_[ver_count -up_idx] = Vec3(0, -r, 0);
                continue;
            }

            auto cur_r = static_cast<real_t >(r * sin(M_PI_2 / row * i));
            real_t y = sqrt(r * r - cur_r * cur_r);
            for (int j = 0; j < col; ++j) {
                auto z = static_cast<real_t>(sin(M_PI * 2 / col * j) * cur_r);
                auto x = static_cast<real_t>(cos(M_PI * 2 / col * j) * cur_r);
                mesh->positions_[up_idx++] = Vec3(x, y, z);
                mesh->positions_[ver_count -up_idx] = Vec3(x, -y, z);
            }
        }
        //中心线
        for (int i = 0; i < col; ++i) {
            float z = sin(angel_step * i) * r;
            float x = cos(angel_step * i) * r;
            mesh->positions_[up_idx++] = Vec3(x,0,z);
        }

        for(int i=0; i < row;i++){
            int base_idx,base_idx_2;
            unsigned a,b,c,d;
            if(i==0){
                base_idx = 1;
                for(int j=0;j<col;j++){
                    a = 0;
                    b = base_idx+j%col;
                    c = base_idx+(j+1)%col;
                    mesh->faces_.push_back({a,b,c});


                    mesh->faces_.push_back({ver_count -1-a,ver_count -1-c,ver_count -1-b});
                }
                continue;
            }

            /*
             * a---b
             * |\  |
             * | \ |
             * |  \|
             * c---d
             */
            base_idx = 1 + (i - 1) * col;
            base_idx_2 = base_idx + col;
            for(int j=0;j<col;j++){
                a = base_idx+j%col;
                b = base_idx+(j+1)%col;
                c = base_idx_2+j%col;
                d = base_idx_2+(j+1)%col;

                mesh->faces_.push_back({a,c,b});
                mesh->faces_.push_back({a,d,b});

                if(i!=row-1){
                /*
                 * a---b      -c-- -d
                 * |\  |       |\   |
                 * | \ |------>| \  |
                 * |  \|       |  \ |
                 * c---d      -a-- -b
                 */
                    mesh->faces_.push_back({ver_count -1-c,ver_count -1-a,ver_count -1-b});
                    mesh->faces_.push_back({ver_count -1-c,ver_count -1-b,ver_count -1-d});
                }
                else{
                /*
                 * a---b       c----d
                 * |\  |       |\   |
                 * | \ |------>| \  |
                 * |  \|       |  \ |
                 * c---d      -a-- -b
                 */
                    mesh->faces_.push_back({c,ver_count -1-a,ver_count -1-b});
                    mesh->faces_.push_back({c,ver_count -1-b,d});
                }
            }
        }
        Transform t;
        t.setTranslate(pos);
        mesh->setTransform(t);
        return mesh;
}
