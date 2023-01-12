from rest_framework.routers import APIRootView


class UsecaseRootView(APIRootView):
    """
    UseCase API root view
    """

    def get_view_name(self):
        return 'Usecase'


#
# Usecase
#
