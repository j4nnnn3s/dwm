#!/usr/bin/python3
from todoist_api_python.api import TodoistAPI
import dmenu

api = TodoistAPI("")

task = dmenu.show([], prompt="Todo: ")
print(api.add_task(content=task, project_id=""))
