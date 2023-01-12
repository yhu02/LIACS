#########################
#                       #
#   Required settings   #
#                       #
#########################

# This is a list of enabled applications to run on this server. ngUML by default does
# not run the runtime. It only runs the modeling and extraction applications. This can
# be used to spawn the "runtime" application. If you want to run all the NLP locally in
# the same thread, you can add the NLP here.
#
# For example, if you want to run the MODEL API, EXTRACTION API & CLASSES NLP, you can
# use the following config: NGUML_APPLICATIONS = ['model', 'extraction', 'classes']
#
# You can also run the EXTRACTION API but dispatch the actual NLP-pipelines through a
# message queue:
# - Run a RQ Worker with NGUML_APPLICATIONS = ['classes']
# - Run the Django ASGI with NGUML_APPLICATIONS = ['model', 'extraction']
#
# You can also use the RQ Workers that we run on the cluster, but this is unstable and
# not thoroughly tested. In this case, you just point your REDIS->"tasks" to the cluster
# Redis, and:
# - NGUML_APPLICATIONS = ['model', 'extraction']
NGUML_APPLICATIONS = ['model']

# This is a list of valid fully-qualified domain names (FQDNs) for the ngUML server. ngUML will not permit write
# access to the server via any other hostnames. The first FQDN in the list will be treated as the preferred name.
#
# Example: ALLOWED_HOSTS = ['nguml.example.com', 'nguml.internal.local']
ALLOWED_HOSTS = ["*"]

# PostgreSQL database configuration. See the Django documentation for a complete list of available parameters:
#   https://docs.djangoproject.com/en/stable/ref/settings/#databases
DATABASE = {
    "NAME": "nguml",  # Database name
    "USER": "nguml",  # PostgreSQL username
    "PASSWORD": "nguml",  # PostgreSQL password
    "HOST": "localhost",  # Database server
    "PORT": "",  # Database port (leave blank for default)
    "CONN_MAX_AGE": 300,  # Max database connection age
}

# Redis database settings. Redis is used for caching and for queuing background tasks such as webhook events. A separate
# configuration exists for each. Full connection details are required in both sections, and it is strongly recommended
# to use two separate database IDs.
REDIS = {
    "tasks": {
        "HOST": "localhost",
        "PORT": 6379,
        # Comment out `HOST` and `PORT` lines and uncomment the following if using Redis Sentinel
        # 'SENTINELS': [('mysentinel.redis.example.com', 6379)],
        # 'SENTINEL_SERVICE': 'nguml',
        "PASSWORD": "nguml",
        "DATABASE": 0,
        "SSL": False,
        # Set this to True to skip TLS certificate verification
        # This can expose the connection to attacks, be careful
        # 'INSECURE_SKIP_TLS_VERIFY': False,
    },
    "caching": {
        "HOST": "localhost",
        "PORT": 6379,
        # Comment out `HOST` and `PORT` lines and uncomment the following if using Redis Sentinel
        # 'SENTINELS': [('mysentinel.redis.example.com', 6379)],
        # 'SENTINEL_SERVICE': 'nguml',
        "PASSWORD": "nguml",
        "DATABASE": 1,
        "SSL": False,
        # Set this to True to skip TLS certificate verification
        # This can expose the connection to attacks, be careful
        # 'INSECURE_SKIP_TLS_VERIFY': False,
    },
}

# This key is used for secure generation of random numbers and strings. It must never be exposed outside of this file.
# For optimal security, SECRET_KEY should be at least 50 characters in length and contain a mix of letters, numbers, and
# symbols. ngUML will not run without this defined. For more information, see
# https://docs.djangoproject.com/en/stable/ref/settings/#std:setting-SECRET_KEY
SECRET_KEY = "1185010326fb91c64c09d085d924128740b45478246d9ee42025070a0fc72be4"


#########################
#                       #
#   Optional settings   #
#                       #
#########################

# Specify one or more name and email address tuples representing ngUML administrators. These people will be notified of
# application errors (assuming correct email settings are provided).
ADMINS = [
    # ('John Doe', 'jdoe@example.com'),
]

# Base URL path if accessing ngUML within a directory. For example, if installed at https://example.com/nguml/, set:
# BASE_PATH = 'nguml/'
BASE_PATH = ""

# API Cross-Origin Resource Sharing (CORS) settings. If CORS_ORIGIN_ALLOW_ALL is set to True, all origins will be
# allowed. Otherwise, define a list of allowed origins using either CORS_ORIGIN_WHITELIST or
# CORS_ORIGIN_REGEX_WHITELIST. For more information, see https://github.com/ottoyiu/django-cors-headers
CORS_ORIGIN_ALLOW_ALL = True
CORS_ORIGIN_WHITELIST = [
    # 'https://hostname.example.com',
]
CORS_ORIGIN_REGEX_WHITELIST = [
    # r'^(https?://)?(\w+\.)?example\.com$',
]

# Set to True to enable server debugging. WARNING: Debugging introduces a substantial performance penalty and may reveal
# sensitive information about your installation. Only enable debugging while performing testing. Never enable debugging
# on a production system.
DEBUG = True

# Email settings
EMAIL = {
    "SERVER": "localhost",
    "PORT": 25,
    "USERNAME": "",
    "PASSWORD": "",
    "USE_SSL": False,
    "USE_TLS": False,
    "TIMEOUT": 10,  # seconds
    "FROM_EMAIL": "",
}

# HTTP proxies ngUML should use when sending outbound HTTP requests (e.g. for webhooks).
# HTTP_PROXIES = {
#     'http': 'http://10.10.1.10:3128',
#     'https': 'http://10.10.1.10:1080',
# }

# IP addresses recognized as internal to the system. The debugging toolbar will be available only to clients accessing
# ngUML from an internal IP.
INTERNAL_IPS = ("127.0.0.1", "::1")

# Enable custom logging. Please see the Django documentation for detailed guidance on configuring custom logs:
#   https://docs.djangoproject.com/en/stable/topics/logging/
LOGGING = {}

# Enable installed plugins. Add the name of each plugin to the list.
PLUGINS = []

# Plugins configuration settings. These settings are used by various plugins that the user may have installed.
# Each key in the dictionary is the name of an installed plugin and its value is a dictionary of settings.
# PLUGINS_CONFIG = {
#     'my_plugin': {
#         'foo': 'bar',
#         'buzz': 'bazz'
#     }
# }

# This repository is used to check whether there is a new release of ngUML available. Set to None to disable the
# version check or use the URL below to check for release in the official ngUML repository.
RELEASE_CHECK_URL = None
# RELEASE_CHECK_URL = 'https://api.github.com/repos/nguml/nguml/releases'

# Maximum execution time for background tasks, in seconds.
RQ_DEFAULT_TIMEOUT = 300

# The file path where custom scripts will be stored. A trailing slash is not needed. Note that the default value of
# this setting is derived from the installed location.
# SCRIPTS_ROOT = '/opt/nguml/nguml/scripts'

# The name to use for the session cookie.
SESSION_COOKIE_NAME = "sessionid"

# By default, ngUML will store session data in the database. Alternatively, a file path can be specified here to use
# local file storage instead. (This can be useful for enabling authentication on a standby instance with read-only
# database access.) Note that the user as which ngUML runs must have read and write permissions to this path.
SESSION_FILE_PATH = None

# Time zone (default: UTC)
TIME_ZONE = "UTC"

# Date/time formatting. See the following link for supported formats:
# https://docs.djangoproject.com/en/stable/ref/templates/builtins/#date
DATE_FORMAT = "N j, Y"
SHORT_DATE_FORMAT = "Y-m-d"
TIME_FORMAT = "g:i a"
SHORT_TIME_FORMAT = "H:i:s"
DATETIME_FORMAT = "N j, Y g:i a"
SHORT_DATETIME_FORMAT = "Y-m-d H:i"

# NLP Servers
ALLENNLP_SERVER = "https://allennlp.semax.nguml.com"
CORENLP_SERVER = "https://corenlp.semax.nguml.com"
