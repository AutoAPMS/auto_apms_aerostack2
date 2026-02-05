# auto_apms_aerostack2

This repo adds plugins to the [AutoAPMS](https://github.com/AutoAPMS/auto-apms) framework to support [AeroStack2](https://github.com/aerostack2/aerostack2) when creating autonomous missions for aerial robotics using behavior trees.

## Usage

To define a behavior using the nodes provided by this package, you link them against your behavior like this:

```cmake
auto_apms_behavior_tree_register_trees( 
  "behavior/my_tree.xml"
  NODE_MANIFEST
  auto_apms_aerostack2::behavior_tree_nodes
)
```
