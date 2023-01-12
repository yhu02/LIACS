# This file monkey patches a recommendation
# from https://github.com/alanjds/drf-nested-routers/issues/147#issuecomment-515318589
# into rest_framework_nested
from rest_framework_nested import routers


class NestedMixin(routers.NestedMixin):
    @property
    def nest_prefix(self):
        return ""

    @nest_prefix.setter
    def nest_prefix(self, value):
        pass

    def check_valid_name(self, value):
        return True


class NestedPatchedRouter(NestedMixin, routers.SimpleRouter):
    pass
