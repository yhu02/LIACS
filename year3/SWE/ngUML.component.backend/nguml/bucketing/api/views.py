from rest_framework.routers import APIRootView


class BucketingRootView(APIRootView):
    """
    Bucketing API root view
    """

    def get_view_name(self):
        return 'Bucketing'


#
# Bucketing
#
