# mosaicgnss-dev

My ROS driver development environment for the Septentrio Mosiac GNSS module.

## Setup

- Clone repository

```bash
git clone https://github.com/ashwin5059198/mosaicgnss-dev.git
cd mosaicgnss-dev
```

- Build required images

```bash
docker-compose build
```

- Start services

```bash
docker-compose up
```

- Run bash in a running container

In a new shell, run

```bash
./shell.sh
```

## Other

* The `shell.sh` executable script can be used a shell replacement. By default it tries to spawn a shell from a running container. If unsuccessful, it spawns a bash shell from root system instead.

eg: for VSCode, in the `settings.json` file, add the following line:

```json
{
    "terminal.integrated.shell.linux": "${workspaceFolder}/shell.sh"
}
```
