<h1 align="center">
  ngUML.backend
</h1>
<p align="center">Extract UML-metadata from natural language, convert it to Django Models and generate runtime applications.</p>

## ⚡️ Quick start

> ☝️ This project is mainly run on GNU/Linux systems. If your main OS is macOS or Windows NT, you can (respectively) 
> use Lima or WSL2 to run it. Docker should suffice on macOS, for Windows, scroll down for information on WSL2.  

First, [download](https://www.python.org/downloads/) and install **Python**. Version `3.8` or higher is required.

You might need to additionally download or add the required dependencies to create virtual environments. 
You can test this with the command: `python -m ensurepip`. In some cases you will need to install the
`virtualenv` package from the package repositories. Usually, for Debian / Ubuntu setups, this can be
done by running `apt update && apt install python3-virtualenv`.

Then, have a Redis & PostgreSQL database ready, the easiest way to do this is using a container runtime such
as [Docker](https://docs.docker.com/get-docker/), [Podman](https://podman-desktop.io/) or 
[Containerd](https://github.com/containerd/nerdctl). 

For Docker, ensure that you have [compose support](https://docs.docker.com/compose/install/).

Then, clone the project, start the databases and start the main server:

```bash
# Ensure that you have the required tools
python --version
python -m ensurepip
docker -v 
docker compose version

# Clone the project
git clone git@github.com:ludev-ng-nl/ngUML.backend.git
cd ./ngUML.backend

# Start the databases
docker compose up -d

# Install all dependencies inside a virtualenv
./upgrade.sh

# Enter the virtualenv 
source venv/bin/activate

# Run the server
./nguml/manage.py runserver
```

> 🔔 Please note: the default configuration in `contrib/configuration.py` starts only the modeling
> application, to run NLP-pipelines, add them to `NGUML_APPLICATIONS` and re-run the `./upgrade.sh`.

If you want to run an NLP-worker, you can run `rqworker` in another window:

```bash
# Enter the virtualenv
source venv/bin/activate

# Run the worker for the queues you need (in the example, models & usecase)
./nguml/manage.py rqworker models usecase
```  

## 📖 Project Documentation

Documentation is written in the `docs` folder and is automatically published to [docs.nguml.com](https://docs.nguml.com).

## ⚙️ Project Applications

The project is structured as a [Django-project with multiple applications](https://docs.djangoproject.com/en/3.2/ref/applications/), 
the main applications are:

### Model

The Model-application contains all the ORM-related objects to store UML-metadata and to
provide the `ngUML.editor` with diagrams and endpoints to edit diagrams.

### Extraction

The Extraction-application contains the endpoints and screens for NLP-based 
UML generation, it doesn't do any generation on itself but adds the necessary
NLP-tasks with metadata to the queue and converts the output from the NLP 
pipeline to ORM-objects.

It currently comes with four major pipelines:
- UML Class Diagrams
- UML Activity Diagrams
- UML Use Case Diagrams
- Bucketing (splitting one big text into different base diagrams)

A pipeline can be started, if the necessary requirements are installed 
(check the `NGUML_APPLICATIONS` and run `upgrade.sh`) by running
`./nguml/manage.py rqworker` for the pipelines to run (i.e. `classes`, `activity`, `usecase`, `bucketing`).

For example:
```bash
# Run a worker that picks up both Classes NLP and Activity NLP
./nguml/manage.py rqworker classes activity
```

### Runtime

This application contains the logic to generate applications from the objects stored
in Model.

## WSL 2 / Windows installation

Check out this helpful guide on [setting up a WSL development environment](https://docs.microsoft.com/en-us/windows/wsl/setup/environment), it should cover all necessities (and more). It may be helpful to check out the documentation below as well. 

There are problems with line endings in windows, so at the moment we use WSL. To get the nguml stack working on windows you will have to make use of WSL. First make sure it your distro is working. [WSL Installation](https://docs.microsoft.com/en-us/windows/wsl/install)

Then after this we are going to continue in the WSL. Open a terminal with WSL. Install git here and make sure your git credentials are working. Then create a folder where you will store the nguml repos. Clone the nguml backend repo into this folder. 

Then open VS Code and go through the following steps to make sure you can use WSL in your VS Code editor. [WSL in VS Code](https://code.visualstudio.com/docs/remote/wsl)

Open the ngUML project that you would like to edit in a WSL editor. If you open a terminal here, you will be working in the correct folder and with the correct Linux distro. Now you can run all your commands just as they should. You will also not have problems with CRLF, because you are working on Linux

## Development Tools
There are a number of visual tools that can be helpful during development:

A visual database model to view all the tables and their relationship:
```http://localhost:8000/dev/database/```
![Database model viewer](https://user-images.githubusercontent.com/24565835/174280716-b893119c-690f-4838-835f-b1d91886e54c.png)

A list of implemented external APIs and their usage:
```http://localhost:8000/dev/schema/swagger-ui```
![API Viewer](https://user-images.githubusercontent.com/24565835/174280711-aa4e48db-7fc1-481e-bdb2-d7f74a4297b0.png)

Both tools are generated automatically, so when new items or changes are made they are automatically updated.

## Debugging

The debugging of the backend repo is dependent on the type of editor. Feel free to add more, if your preferred editor is not there.

### VS Code
The debugging is partially based on [this section](https://code.visualstudio.com/docs/containers/quickstart-python#_build-run-and-debug-the-container) from the VS code website, we changed some things to accompany our docker configuration.

To debug the code in visual studio code one can use the following a docker compose command and the visual studio code debugger. First one must add the following configuration to the file .vscode/launch.json :
```
"configurations": [
        {
            "name": "Python: Remote Attach",
            "type": "python",
            "request": "attach",
            "connect": {
                "host": "localhost",
                "port": 5678
            },
            "pathMappings": [
                {
                    "localRoot": "${workspaceFolder}/src",
                    "remoteRoot": "/code"
                }
            ]
        }
    ]
```

The launch.json made it possible to attach the visual studio debugger to the django docker container. 

Next to that we need to add the python package debugpy to the container. This is already specified in the docker-compose.debug.yml file. It is installed in the start command of the django container in the docker-compose.debug file, where we added `pip install debugpy -t`. Next to that we start the container with the debugger using the following command `python /tmp/debugpy`. Now we start the docker containers, for that we use the following command:
```
docker compose -f docker-compose.debug.yml up
```

The docker compose debugging containers will start. Once it stops doing installs and other startup related things it will wait for the debugger to attach, the progress can be followed in the terminal. To attach the debugger you can open the debugger tab in the sidebar of vs code, it has the play and bug logo on it.

Then you have to select the "Python: Remote Attach debugger", that has our configuration in it. When you press on play the configuration will attach the debugger to the django container. 

To really start debugging you will need to add a breakpoint in your code where you want the debugger to stop. Finally you must make sure the code is executed to hit the breakpoint, this can be done through an API call or navigating to a certain URL. If you did this correctly you will stop at the breakpoint and can use the debug console to run python code while you have stopped and inspect the different variables.

## NLP models

## Activity NLP models
The NLP models are taken from AllenNLP, currently the Coreference [[1]](#1), Semantic Role Labelling [[2]](#2) and Entailment [[3]](#3) models are used. They were taken from the [AllenNLP](https://allennlp.org/) website. 

### Upgrading models
If the models need to be upgraded in the future there are a few places where the models need to be update. 

1. [allen_nlp/allen_models_download.sh](allen_models_download.sh)
2. [allen_nlp/src/app.py](allen_nlp/src/app.py)

In download_models the url to the downloads need to be changed and in app.py the link to the files need to be changed. That way one is able to upgrade the models if needed.

## Handy tools
Using the save and load function from the numpy library allows us to save dictionaries temporarily, such that we can reuse them and don't need to re-rerun everything everytime we restart. Example from [Delftstack](https://www.delftstack.com/howto/python/python-save-dictionary/#save-a-dictionary-to-file-in-python-using-the-save-function-of-numpy-library)

Saving:
```
import numpy as np

my_dict = { 'Apple': 4, 'Banana': 2, 'Orange': 6, 'Grapes': 11}
np.save('file.npy', my_dict)
```
loading:
```
import numpy as np

new_dict = np.load('file.npy', allow_pickle='TRUE')
print(new_dict.item())
```

# References
<a id="1">[1]</a> 
Lee, K., He, L., & Zettlemoyer, L. (2018). Higher-Order Coreference Resolution with Coarse-to-Fine Inference. NAACL.

<a id="2">[2]</a>
Shi, P., & Lin, J.J. (2019). Simple BERT Models for Relation Extraction and Semantic Role Labeling. ArXiv, abs/1904.05255.

<a id="3">[3]</a>
Liu, Y., Ott, M., Goyal, N., Du, J., Joshi, M., Chen, D., Levy, O., Lewis, M., Zettlemoyer, L., & Stoyanov, V. (2019). RoBERTa: A Robustly Optimized BERT Pretraining Approach. ArXiv, abs/1907.11692.
