## Boxes Test

This is a small module dedicated to Box plugin experimentation.

The existing plugin is a first stab at a Simple Views Box, where the first 
result of a View is rendered as the content, and a link may be provided (or 
generated, in the case of Page views) to view more such content.

If this module were to advance for actual usage, I would want to create a 
utility plugin for all Views-centric boxes with some utilities (such as the 
two functions already in the views_simple plugin here). Then extend other 
plugins to do various things.

However, the usefulness of a Views Box system is pretty shaky, given that all 
it's really doing is duplicating (in a locally configurable way) the 
functionality of a Views block display. More relevant would be a plugin that 
is tied to a specific View, and allows the user to specify arguments or 
filter parameters.

### Todos
If you are a zany person, and decide to clean this up for real use somewhere, 
the views_simple plugin does not perform an access check at this time.