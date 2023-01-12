import importlib
import logging
import os
import platform
import re
import socket
import sys
import warnings
from urllib.parse import urlsplit

from django.contrib.messages import constants as messages
from django.core.exceptions import ImproperlyConfigured, ValidationError
from django.core.validators import URLValidator

from nguml.config import PARAMS

#
# Environment setup
#

VERSION = "1.0.0-rc1"

# Hostname
HOSTNAME = platform.node()

# Set the base directory two levels up
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Validate Python version
if sys.version_info < (3, 7):
    raise RuntimeError(f"NGUML requires Python 3.7 or later. (Currently installed: Python {platform.python_version()})")

#
# Configuration import
#

# Import configuration parameters
try:
    from nguml import configuration
except ModuleNotFoundError as e:
    if getattr(e, "name") == "configuration":
        raise ImproperlyConfigured(
            "Configuration file is not present. Please define nguml/nguml/configuration.py per the documentation."
        )
    raise

# Enforce required configuration parameters
for parameter in [
    "NGUML_APPLICATIONS",
    "ALLOWED_HOSTS",
    "DATABASE",
    "SECRET_KEY",
    "REDIS",
]:
    if not hasattr(configuration, parameter):
        raise ImproperlyConfigured("Required parameter {} is missing from configuration.py.".format(parameter))

# Set required parameters
NGUML_APPLICATIONS = getattr(configuration, "NGUML_APPLICATIONS")
ALLOWED_HOSTS = getattr(configuration, "ALLOWED_HOSTS")
DATABASE = getattr(configuration, "DATABASE")
REDIS = getattr(configuration, "REDIS")
SECRET_KEY = getattr(configuration, "SECRET_KEY")

# Set static config parameters
ADMINS = getattr(configuration, "ADMINS", [])
BASE_PATH = getattr(configuration, "BASE_PATH", "")
if BASE_PATH:
    BASE_PATH = BASE_PATH.strip("/") + "/"  # Enforce trailing slash only
CORS_ORIGIN_ALLOW_ALL = getattr(configuration, "CORS_ORIGIN_ALLOW_ALL", False)
CORS_ORIGIN_REGEX_WHITELIST = getattr(configuration, "CORS_ORIGIN_REGEX_WHITELIST", [])
CORS_ORIGIN_WHITELIST = getattr(configuration, "CORS_ORIGIN_WHITELIST", [])
DATE_FORMAT = getattr(configuration, "DATE_FORMAT", "N j, Y")
DATETIME_FORMAT = getattr(configuration, "DATETIME_FORMAT", "N j, Y g:i a")
DEBUG = getattr(configuration, "DEBUG", False)
DEVELOPER = getattr(configuration, "DEVELOPER", False)
DOCS_ROOT = getattr(configuration, "DOCS_ROOT", os.path.join(os.path.dirname(BASE_DIR), "docs"))
EMAIL = getattr(configuration, "EMAIL", {})
EXEMPT_VIEW_PERMISSIONS = getattr(configuration, "EXEMPT_VIEW_PERMISSIONS", [])
HTTP_PROXIES = getattr(configuration, "HTTP_PROXIES", None)
INTERNAL_IPS = getattr(configuration, "INTERNAL_IPS", ("127.0.0.1", "::1"))
LOGGING = getattr(configuration, "LOGGING", {})
LOGIN_PERSISTENCE = getattr(configuration, "LOGIN_PERSISTENCE", False)
LOGIN_REQUIRED = getattr(configuration, "LOGIN_REQUIRED", False)
LOGIN_TIMEOUT = getattr(configuration, "LOGIN_TIMEOUT", None)
MEDIA_ROOT = getattr(configuration, "MEDIA_ROOT", os.path.join(BASE_DIR, "media")).rstrip("/")
PLUGINS = getattr(configuration, "PLUGINS", [])
PLUGINS_CONFIG = getattr(configuration, "PLUGINS_CONFIG", {})
RELEASE_CHECK_URL = getattr(configuration, "RELEASE_CHECK_URL", None)
REPORTS_ROOT = getattr(configuration, "REPORTS_ROOT", os.path.join(BASE_DIR, "reports")).rstrip("/")
RQ_DEFAULT_TIMEOUT = getattr(configuration, "RQ_DEFAULT_TIMEOUT", 300)
SCRIPTS_ROOT = getattr(configuration, "SCRIPTS_ROOT", os.path.join(BASE_DIR, "scripts")).rstrip("/")
SESSION_FILE_PATH = getattr(configuration, "SESSION_FILE_PATH", None)
SESSION_COOKIE_NAME = getattr(configuration, "SESSION_COOKIE_NAME", "sessionid")
SHORT_DATE_FORMAT = getattr(configuration, "SHORT_DATE_FORMAT", "Y-m-d")
SHORT_DATETIME_FORMAT = getattr(configuration, "SHORT_DATETIME_FORMAT", "Y-m-d H:i")
SHORT_TIME_FORMAT = getattr(configuration, "SHORT_TIME_FORMAT", "H:i:s")
STORAGE_BACKEND = getattr(configuration, "STORAGE_BACKEND", None)
STORAGE_CONFIG = getattr(configuration, "STORAGE_CONFIG", {})
TIME_FORMAT = getattr(configuration, "TIME_FORMAT", "g:i a")
TIME_ZONE = getattr(configuration, "TIME_ZONE", "UTC")

# Check for hard-coded dynamic config parameters
for param in PARAMS:
    if hasattr(configuration, param.name):
        globals()[param.name] = getattr(configuration, param.name)

#
# Database
#

# Only PostgreSQL is supported
DATABASE.update({"ENGINE": "django.db.backends.postgresql"})
# Atomic GraphQL updates through Graphene
DATABASE.update({"ATOMIC_MUTATIONS": True})

DATABASES = {
    "default": DATABASE,
}

#
# Redis
#

# Background task queuing
if "tasks" not in REDIS:
    raise ImproperlyConfigured("REDIS section in configuration.py is missing the 'tasks' subsection.")
TASKS_REDIS = REDIS["tasks"]
TASKS_REDIS_HOST = TASKS_REDIS.get("HOST", "localhost")
TASKS_REDIS_PORT = TASKS_REDIS.get("PORT", 6379)
TASKS_REDIS_SENTINELS = TASKS_REDIS.get("SENTINELS", [])
TASKS_REDIS_USING_SENTINEL = all([isinstance(TASKS_REDIS_SENTINELS, (list, tuple)), len(TASKS_REDIS_SENTINELS) > 0])
TASKS_REDIS_SENTINEL_SERVICE = TASKS_REDIS.get("SENTINEL_SERVICE", "default")
TASKS_REDIS_SENTINEL_TIMEOUT = TASKS_REDIS.get("SENTINEL_TIMEOUT", 10)
TASKS_REDIS_PASSWORD = TASKS_REDIS.get("PASSWORD", "")
TASKS_REDIS_DATABASE = TASKS_REDIS.get("DATABASE", 0)
TASKS_REDIS_SSL = TASKS_REDIS.get("SSL", False)
TASKS_REDIS_SKIP_TLS_VERIFY = TASKS_REDIS.get("INSECURE_SKIP_TLS_VERIFY", False)

# Caching
if "caching" not in REDIS:
    raise ImproperlyConfigured("REDIS section in configuration.py is missing caching subsection.")
CACHING_REDIS_HOST = REDIS["caching"].get("HOST", "localhost")
CACHING_REDIS_PORT = REDIS["caching"].get("PORT", 6379)
CACHING_REDIS_DATABASE = REDIS["caching"].get("DATABASE", 0)
CACHING_REDIS_PASSWORD = REDIS["caching"].get("PASSWORD", "")
CACHING_REDIS_SENTINELS = REDIS["caching"].get("SENTINELS", [])
CACHING_REDIS_SENTINEL_SERVICE = REDIS["caching"].get("SENTINEL_SERVICE", "default")
CACHING_REDIS_PROTO = "rediss" if REDIS["caching"].get("SSL", False) else "redis"
CACHING_REDIS_SKIP_TLS_VERIFY = REDIS["caching"].get("INSECURE_SKIP_TLS_VERIFY", False)

CACHES = {
    "default": {
        "BACKEND": "django_redis.cache.RedisCache",
        "LOCATION": f"{CACHING_REDIS_PROTO}://{CACHING_REDIS_HOST}:{CACHING_REDIS_PORT}/{CACHING_REDIS_DATABASE}",
        "OPTIONS": {
            "CLIENT_CLASS": "django_redis.client.DefaultClient",
            "PASSWORD": CACHING_REDIS_PASSWORD,
        },
    }
}
if CACHING_REDIS_SENTINELS:
    DJANGO_REDIS_CONNECTION_FACTORY = "django_redis.pool.SentinelConnectionFactory"
    CACHES["default"]["LOCATION"] = f"{CACHING_REDIS_PROTO}://{CACHING_REDIS_SENTINEL_SERVICE}/{CACHING_REDIS_DATABASE}"
    CACHES["default"]["OPTIONS"]["CLIENT_CLASS"] = "django_redis.client.SentinelClient"
    CACHES["default"]["OPTIONS"]["SENTINELS"] = CACHING_REDIS_SENTINELS
if CACHING_REDIS_SKIP_TLS_VERIFY:
    CACHES["default"]["OPTIONS"].setdefault("CONNECTION_POOL_KWARGS", {})
    CACHES["default"]["OPTIONS"]["CONNECTION_POOL_KWARGS"]["ssl_cert_reqs"] = False


#
# Sessions
#

if LOGIN_TIMEOUT is not None:
    # Django default is 1209600 seconds (14 days)
    SESSION_COOKIE_AGE = LOGIN_TIMEOUT
SESSION_SAVE_EVERY_REQUEST = bool(LOGIN_PERSISTENCE)
if SESSION_FILE_PATH is not None:
    SESSION_ENGINE = "django.contrib.sessions.backends.file"


#
# Email
#

EMAIL_HOST = EMAIL.get("SERVER")
EMAIL_HOST_USER = EMAIL.get("USERNAME")
EMAIL_HOST_PASSWORD = EMAIL.get("PASSWORD")
EMAIL_PORT = EMAIL.get("PORT", 25)
EMAIL_SSL_CERTFILE = EMAIL.get("SSL_CERTFILE")
EMAIL_SSL_KEYFILE = EMAIL.get("SSL_KEYFILE")
EMAIL_SUBJECT_PREFIX = "[nguml] "
EMAIL_USE_SSL = EMAIL.get("USE_SSL", False)
EMAIL_USE_TLS = EMAIL.get("USE_TLS", False)
EMAIL_TIMEOUT = EMAIL.get("TIMEOUT", 10)
SERVER_EMAIL = EMAIL.get("FROM_EMAIL")


#
# Django
#

INSTALLED_APPS = (
    [
        "django.contrib.admin",
        "django.contrib.auth",
        "django.contrib.contenttypes",
        "django.contrib.sessions",
        "django.contrib.messages",
        "django.contrib.staticfiles",
        "django.contrib.humanize",
        "clearcache",
        "daphne",
        "corsheaders",
        "debug_toolbar",
        "graphiql_debug_toolbar",
        "django_filters",
        "django_tables2",
        "graphene_django",
        "mptt",
        "taggit",
        "timezone_field",
        "rest_framework",
        "schema_graph",
        "drf_spectacular",
        "nguml",
    ]
    + getattr(configuration, "NGUML_APPLICATIONS", ["model"])
    + [
        "django_rq",  # Must come after extras to allow overriding management commands
    ]
)

# Middleware
MIDDLEWARE = [
    "graphiql_debug_toolbar.middleware.DebugToolbarMiddleware",
    "corsheaders.middleware.CorsMiddleware",
    "django.contrib.sessions.middleware.SessionMiddleware",
    "django.middleware.common.CommonMiddleware",
    "django.middleware.csrf.CsrfViewMiddleware",
    "django.contrib.auth.middleware.AuthenticationMiddleware",
    "django.contrib.messages.middleware.MessageMiddleware",
    "django.middleware.clickjacking.XFrameOptionsMiddleware",
    "django.middleware.security.SecurityMiddleware",
]

ROOT_URLCONF = "nguml.urls"

TEMPLATES_DIR = BASE_DIR + "/templates"
TEMPLATES = [
    {
        "BACKEND": "django.template.backends.django.DjangoTemplates",
        "DIRS": [TEMPLATES_DIR],
        "APP_DIRS": True,
        "OPTIONS": {
            "context_processors": [
                "django.template.context_processors.debug",
                "django.template.context_processors.request",
                "django.template.context_processors.media",
                "django.contrib.auth.context_processors.auth",
                "django.contrib.messages.context_processors.messages",
            ],
        },
    },
]

# Internationalization
LANGUAGE_CODE = "en-us"
USE_I18N = True
USE_TZ = True

# WSGI
WSGI_APPLICATION = 'nguml.wsgi.application'
ASGI_APPLICATION = 'nguml.asgi.application'
SECURE_PROXY_SSL_HEADER = ("HTTP_X_FORWARDED_PROTO", "https")
USE_X_FORWARDED_HOST = True
X_FRAME_OPTIONS = "SAMEORIGIN"

# Static files (CSS, JavaScript, Images)
STATIC_ROOT = BASE_DIR + "/static"
STATIC_URL = f"/{BASE_PATH}static/"
STATICFILES_DIRS = (
    os.path.join(BASE_DIR, "project-static", "dist"),
    os.path.join(BASE_DIR, "project-static", "img"),
    ("docs", os.path.join(BASE_DIR, "project-static", "docs")),  # Prefix with /docs
)

# Media
MEDIA_URL = "/{}media/".format(BASE_PATH)

CSRF_TRUSTED_ORIGINS = ALLOWED_HOSTS

DEFAULT_AUTO_FIELD = "django.db.models.AutoField"

#
# Django filters
#

FILTERS_NULL_CHOICE_LABEL = "None"
FILTERS_NULL_CHOICE_VALUE = "null"

#
# Graphene
#
GRAPHENE = {
    # Avoids naming collision on models with 'type' field; see
    # https://github.com/graphql-python/graphene-django/issues/185
    "DJANGO_CHOICE_FIELD_ENUM_V3_NAMING": True,
    "MIDDLEWARE": [
        "graphene_django.debug.DjangoDebugMiddleware",
    ],
}

#
# Django REST Framework
#
REST_FRAMEWORK = {
    'DEFAULT_FILTER_BACKENDS': ['django_filters.rest_framework.DjangoFilterBackend'],
    "DEFAULT_SCHEMA_CLASS": "drf_spectacular.openapi.AutoSchema",
}


SPECTACULAR_SETTINGS = {
    'TITLE': 'ngUML.backend API Explorer',
    'DESCRIPTION': 'API reference, this documentation is automatically generated and therefore updates itself.',
    'VERSION': '1.0.0',
    'SORT_OPERATIONS': False,
    'SERVE_INCLUDE_SCHEMA': True,
}


#
# Django RQ (Webhooks backend)
#

if TASKS_REDIS_USING_SENTINEL:
    RQ_PARAMS = {
        "SENTINELS": TASKS_REDIS_SENTINELS,
        "MASTER_NAME": TASKS_REDIS_SENTINEL_SERVICE,
        "DB": TASKS_REDIS_DATABASE,
        "PASSWORD": TASKS_REDIS_PASSWORD,
        "SOCKET_TIMEOUT": None,
        "CONNECTION_KWARGS": {"socket_connect_timeout": TASKS_REDIS_SENTINEL_TIMEOUT},
    }
else:
    RQ_PARAMS = {
        "HOST": TASKS_REDIS_HOST,
        "PORT": TASKS_REDIS_PORT,
        "DB": TASKS_REDIS_DATABASE,
        "PASSWORD": TASKS_REDIS_PASSWORD,
        "SSL": TASKS_REDIS_SSL,
        "SSL_CERT_REQS": None if TASKS_REDIS_SKIP_TLS_VERIFY else "required",
        "DEFAULT_TIMEOUT": RQ_DEFAULT_TIMEOUT,
    }

RQ_QUEUES = {
    "class": RQ_PARAMS,
    "usecase": RQ_PARAMS,
    "component": RQ_PARAMS,
    "activity": RQ_PARAMS,
    "bucketing": RQ_PARAMS,
    "runtime": RQ_PARAMS,
    "extraction": RQ_PARAMS,
    "model": RQ_PARAMS,
}

RQ_SHOW_ADMIN_LINK = True

#
# Plugins
#

for plugin_name in PLUGINS:

    # Import plugin module
    try:
        plugin = importlib.import_module(plugin_name)
    except ModuleNotFoundError as e:
        if getattr(e, "name") == plugin_name:
            raise ImproperlyConfigured(
                "Unable to import plugin {}: Module not found. Check that the plugin module has been installed within the "
                "correct Python environment.".format(plugin_name)
            )
        raise e

    # Determine plugin config and add to INSTALLED_APPS.
    try:
        plugin_config = plugin.config
        INSTALLED_APPS.append("{}.{}".format(plugin_config.__module__, plugin_config.__name__))
    except AttributeError:
        raise ImproperlyConfigured(
            "Plugin {} does not provide a 'config' variable. This should be defined in the plugin's __init__.py file "
            "and point to the PluginConfig subclass.".format(plugin_name)
        )

    # Validate user-provided configuration settings and assign defaults
    if plugin_name not in PLUGINS_CONFIG:
        PLUGINS_CONFIG[plugin_name] = {}
    plugin_config.validate(PLUGINS_CONFIG[plugin_name], VERSION)

    # Add middleware
    plugin_middleware = plugin_config.middleware
    if plugin_middleware and type(plugin_middleware) in (list, tuple):
        MIDDLEWARE.extend(plugin_middleware)

    # Create RQ queues dedicated to the plugin
    # we use the plugin name as a prefix for queue name's defined in the plugin config
    # ex: mysuperplugin.mysuperqueue1
    if type(plugin_config.queues) is not list:
        raise ImproperlyConfigured("Plugin {} queues must be a list.".format(plugin_name))
    RQ_QUEUES.update({f"{plugin_name}.{queue}": RQ_PARAMS for queue in plugin_config.queues})

TEMPLATES_AUTO_RELOAD = True
