#! sh

fswatch -r ./src \
| xargs -I{} realpath --relative-to=$(realpath .) {} \
| xargs -I{} scp ./{} 'pwa-arm-dev:~/'{}
