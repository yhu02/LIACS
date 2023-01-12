from rest_framework.routers import APIRootView


class ActivityRootView(APIRootView):
    """
    Activity API root view
    """

    def get_view_name(self):
        return 'Activity'


#
# Activity
#
