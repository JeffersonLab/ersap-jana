# JANA ERSAP Integration Example

[![Build Status](https://travis-ci.org/JeffersonLab/ersap-jana.svg?branch=master)](https://travis-ci.org/JeffersonLab/ersap-jana)

Example of a JANA application presenting as a CLARA service.
This is an implementation of the clara::stdlib::StreamingService abstract service, 
that accepts 4 VTP streams and presents at its output aggregated and processed stream of data.

## Application architecture

### `engine` subdirectory

Here will go all supporting JANA classes necessary for stream processing.
The local CmakeList.txt will create an .so artifact that will be used during the 
service creation.
 
### `data` subproject

When the services are developed to be shared with other developers and
investigation groups, it is a good idea to present the data types used by the
services into its own package.

Orchestrators can then add a dependency on the data type package to send
requests to the services, without downloading all the services implementation
dependencies.

So, if JANA team decides to use it's own data type then here will go code 
for that particular data type serialization, with defined mime-type.
The local CmakeList.txt will creat an appropriate shared lib.

### `services` subproject

The final CLARA services are just a tiny layer over the `engine` code.
The most important thing to consider is thread-safety of the engine classes.
CLARA services will run in a multi-thread environment and developers must
ensure thread-safety.

The `service`  CmakeList.txt creates the `libJanaService.so` (or dylib) artifact
that will be used by the CLARA orchestrator.


## Dependencies

and [CLARA C++](https://github.com/JeffersonLab/clara-cpp#build-notes)
into `$CLARA_HOME`.

## Build and deploy

Build and deploy the C++ services into `$CLARA_HOME`:

    > mkdir build
    > cd build
    > cmake ..
    > make
    > make install

## Running the application

### Using the CLARA Shell

Use the `clara-shell` to run the services locally (running on a farm will require artifacts 
being accessible on every farm node).
The shell provides a high-level interface to configure and start
the different CLARA components required to run an application.

1. Start the CLARA shell:

        >$CLARA_HOME/bin/clara-shell

2. Define the application within a `services.yaml` file.
   There is one file ready in the root of the repository.

       clara> set servicesFile services.yaml

3. Optionally you can change the number of parallel threads
   used by the services to process requests

       clara> set threads <NUM_THREADS>

5. Start the data processing.
   This will start the main Java DPE,
   a C++ DPE if the C++ service is listed in `services.yaml`,
   and it will run the streaming orchestrator to process the data-stream.

       clara> run local

You can put the above shell commands into a script and run the script instead.
There is a `jana.clara` script ready in the root of the repository:

      > $CLARA_HOME/bin/clara-shell jana.clara

### Starting the CLARA DPEs and orchestrator manually

All CLARA components started by the CLARA shell can be run manually.

1. Start the main Java DPE:

       > $CLARA_HOME/bin/j_dpe

2. Start the C++ DPE (if the application uses C++ services):

       > $CLARA_HOME/bin/c_dpe --fe-host localhost

3. Define the application with a `services.yaml` file.
   There is one file ready in the root of the repository.
   

4. Run the streaming orchestrator with the application configuration file.

       > $CLARA_HOME/bin/clara-stream-orchestrator -F -i ./resources -o /tmp/out services.yaml 

   To change the number of parallel threads used by the services to process
   requests, pass the `-t <NUM_THREADS>` option.

