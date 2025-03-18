#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    turtle1 = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtle1',
        output='screen',
    )

    turtle2 = Node(
        package='cpp_a_follow_turtle_pkg',
        executable='turtle_spawner',
        name='turtle2',
        output='screen',
    )

    turtle_pub1 = Node(
        package='cpp_a_follow_turtle_pkg',
        executable='turtle_pub',
        name='tf_caster1',
        arguments=['turtle1'],
        output='screen',
    )

    turtle_pub2 = Node(
        package='cpp_a_follow_turtle_pkg',
        executable='turtle_pub',
        name='tf_caster2',
        arguments=['turtle2'],
        output='screen',
    )

    turtle2_controller = Node(
        package='cpp_a_follow_turtle_pkg',
        executable='turtle2_controller',
        name='turtle2_controller',
        output='screen',
    )      

    return LaunchDescription([     
        turtle1,
        turtle2,
        turtle_pub1,
        turtle_pub2,
        turtle2_controller,
    ])