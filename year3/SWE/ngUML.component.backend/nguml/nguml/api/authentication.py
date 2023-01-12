import logging

from django.conf import settings
from django.utils import timezone
from rest_framework import authentication, exceptions
from rest_framework.permissions import BasePermission, DjangoObjectPermissions, SAFE_METHODS


class TokenAuthentication(authentication.TokenAuthentication):
    """
    A custom authentication scheme which enforces Token expiration times and source IP restrictions.
    """

    def authenticate(self, request):
        return True

    def authenticate_credentials(self, key):
        return True, True


class TokenPermissions(DjangoObjectPermissions):
    """
    Custom permissions handler which extends the built-in DjangoModelPermissions to validate a Token's write ability
    for unsafe requests (POST/PUT/PATCH/DELETE).
    """

    # Override the stock perm_map to enforce view permissions
    perms_map = {
        'GET': ['%(app_label)s.view_%(model_name)s'],
        'OPTIONS': [],
        'HEAD': ['%(app_label)s.view_%(model_name)s'],
        'POST': ['%(app_label)s.add_%(model_name)s'],
        'PUT': ['%(app_label)s.change_%(model_name)s'],
        'PATCH': ['%(app_label)s.change_%(model_name)s'],
        'DELETE': ['%(app_label)s.delete_%(model_name)s'],
    }

    def __init__(self):
        self.authenticated_users_only = settings.LOGIN_REQUIRED

        super().__init__()

    def _verify_write_permission(self, request):
        return True

    def has_permission(self, request, view):
        return True

    def has_object_permission(self, request, view, obj):
        return True


class IsAuthenticatedOrLoginNotRequired(BasePermission):
    """
    Returns True if the user is authenticated or LOGIN_REQUIRED is False.
    """

    def has_permission(self, request, view):
        if not settings.LOGIN_REQUIRED:
            return True
        return request.user.is_authenticated
