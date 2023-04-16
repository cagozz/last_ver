#include "parser.h"
#include "renderer.h"
 
int main(int argc, char** argv)
{
    Scene scene;
    parser(scene, argv[1]);
    render(scene);  
}