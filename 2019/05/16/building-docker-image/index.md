
>***Docker images*** are built based on a Dockerfile. A Dockerfile defines **all the steps** required to create a Docker image with your application configured and ready to be run as a container. The image itself contains everything, from operating system to dependencies and configuration required to run your application.
Having everything within the image allows you to migrate images between different environments and be confident that if it works in one environment, then it will work in another.
The Dockerfile allows for images to be composable, enabling users to extend existing images instead of building from scratch. *By building on an existing image, you only need to define the steps to setup your application*. The base images can be basic operating system installations or configured systems which simply need some additional customisations.

# steps

## Base images

> All Docker images start from a base image. A base image is the same images from the Docker Registry which are used to start containers. Along with the image name, we can also include the image tag to indicate which particular version we want, *by default, this is latest*.
These base images are used as the foundation for your additional changes to run your application. For example, in this scenario, we require NGINX to be configured and running on the system before we can deploy our static HTML files. As such we want to use NGINX as our base image.
Dockerfile's are simple text files with a command on each line. To define a base image we use the instruction **FROM image-name:tag**

```Dockerfile
FROM nginx:1.11-alpine
```

## Running commands

>With the base image defined, we need to run various commands to configure our image. There are many commands to help with this, the main commands two are COPY and RUN.
***RUN*** command allows you to execute any command as you would at a command prompt, for example installing different application packages or running a build command. The results of the RUN are **persisted** to the image so it's important not to leave any unnecessary or temporary files on the disk as these will be included in the image.
***COPY src dest*** allows you to copy files from the directory containing the Dockerfile to the container's image. This is extremely useful for **source code** and assets that you want to be deployed inside your container
*A new index.html file has been created for you which we want to serve from our container. On the next line after the ***FROM*** command, use the COPY command to copy index.html into a directory called **/usr/share/nginx/**html*

```Dockerfile
FROM nginx:1.11-alpine
COPY index.html /usr/share/nginx/html/index.html
```

## Exposing ports

>With our files copied into our image and any dependencies downloaded, you need to define which port application needs to be accessible on.
Using the ***EXPOSE port*** command you tell Docker which ports should be open and can be bound too. You can define multiple ports on the single command, for example, EXPOSE 80 433 or EXPOSE 7000-8000

```Dockerfile
FROM nginx:1.11-alpine
COPY index.html /usr/share/nginx/html/index.html
# web server to be accessible via port 80
EXPOSE 80
```

## Default commands

>With the Docker image configured and having defined which ports we want accessible, we now need to define the command that launches the application.
*The ***CMD*** line in a Dockerfile defines the default command to run when a container is launched.* If the command requires arguments then it's recommended to use an array, for example ["cmd", "-a", "arga value", "-b", "argb-value"], which will be combined together and the command 
***cmd -a "arga value" -b argb-value** would be run.
>An alternative approach to CMD is ENTRYPOINT. While a CMD can be overridden when the container starts, a ENTRYPOINT defines a command which can have arguments passed to it when the container launches.

```Dockerfile
FROM nginx:1.11-alpine
COPY index.html /usr/share/nginx/html/index.html
# web server to be accessible via port 80
EXPOSE 80
# The command to run NGINX is nginx -g daemon off;
CMD ["nginx", "-g", "daemon off;"]
```

## Building containers

>After writing your Dockerfile you need to use ***docker build*** to turn it into an image. *The build command takes in a directory containing the Dockerfile, executes the steps and stores the image in your local Docker Engine.* If one fails because of an error then the build stops.
>Using the docker build command to build the image. You can give the image a ***friendly name by using the -t name option.***

```bash
docker build -t my-nginx-image:latest
>Sending build context to Docker daemon  3.072kB
Step 1/5 : FROM nginx:1.11-alpine
 ---> bedece1f06cc
Step 2/5 : COPY index.html /usr/share/nginx/html/index.html
 ---> Using cache
 ---> 2be98924804a
Step 3/5 : EXPOSE 80
 ---> Using cache
 ---> 233ea4308326
Step 4/5 : CMD ["nginx", "-g", "daemon off;"]
 ---> Using cache
 ---> 1a518260bb2b
Step 5/5 : CMD ["nginx", "-g", "daemon off;"]
 ---> Using cache
 ---> 563c68c52ed7
Successfully built 563c68c52ed7
Successfully tagged my-nginx-image:latest
```

## Launching new image

> NGINX is designed to run as a background service so you should include the option -d. To make the web server accessible, bind it to port 80 using p 80:80

```bash
docker run -d -p 80:80 my-nginx-image:latest
```

> access the launched web server via the hostname docker. the command ***curl -i http://docker*** will return our index file via NGINX and the image we built.

```bash
curl -i http://docker
```
