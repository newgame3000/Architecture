 #!/bin/bash
 cmake -B ./build -S ./
 cd build
 make
 mv ApiGateway.out ../