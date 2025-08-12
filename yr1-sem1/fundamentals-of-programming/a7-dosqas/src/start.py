import configparser
import importlib.util
import os
from ui.ui import UI
from services.services import Services


def start():
    """
    Starts the program
    :return:
    """
    while True:
        ui.menu()


config = configparser.ConfigParser()
config.read('settings.properties')
repository_name = config.get('Config', 'repository')
base_directory = 'repository'
module_name = f"{repository_name}"
repository_path = os.path.join(base_directory, f"{repository_name}.py")
spec = importlib.util.spec_from_file_location(module_name, repository_path)
repository_module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(repository_module)
words = repository_name.split("_")
repository_name = ""
for word in words:
    repository_name += word.capitalize()
RepositoryClass = getattr(repository_module, repository_name)

repository_instance = RepositoryClass()
print(f"Using repository: {repository_name}\n")

services = Services(repository_instance)
ui = UI(services)
start()
