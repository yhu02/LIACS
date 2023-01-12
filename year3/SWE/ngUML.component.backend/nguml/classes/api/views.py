from rest_framework.routers import APIRootView


class ClassesRootView(APIRootView):
    """
    Classes API root view
    """

    def get_view_name(self):
        return 'Classes'


#
# Classes
#
