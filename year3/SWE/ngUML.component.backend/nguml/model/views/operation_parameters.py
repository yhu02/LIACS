# from django.shortcuts import render
from django.shortcuts import redirect
from django.views.decorators.csrf import csrf_exempt
from ..models import Operation, OperationParameter
from ..enums import Type


@csrf_exempt
def add(request):
    operation_id = request.POST['operation_id']
    operation = Operation.objects.get(id=operation_id)
    return_type = request.POST['type']
    name = request.POST['name']

    if return_type == 'string':
        return_type = Type.String
    elif return_type == 'int':
        return_type = Type.Int
    elif return_type == 'bool':
        return_type = Type.Bool

    operation_parameter = OperationParameter(name=name, type=return_type, operation=operation)
    operation_parameter.save()
    return redirect('/model/operations/' + operation_id)


@csrf_exempt
def delete(request, operation_parameter_id):
    operation_parameter = OperationParameter.objects.get(id=operation_parameter_id)
    operation = operation_parameter.operation
    operation_parameter.delete()
    return redirect('/model/operations/' + str(operation.id))
