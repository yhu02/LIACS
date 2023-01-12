# ngUML.extraction

ngUML.extraction implements a general frontend
and API-endpoints for the extraction of metadata
from requirements texts.

It populates itself with possible generators using
the `NGUML_APPLICATIONS` setting for now. Later it
might be nice to use a separate setting for this, so
we can also support separate packages. 