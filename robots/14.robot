{
   "backend" : "xl-320",
   "parameters" : {
      "A" : {
         "max" : "45",
         "min" : "-25",
         "val" : "0"
      },
      "BODY" : {
         "max" : "100",
         "min" : "30",
         "val" : "30"
      },
      "L1" : {
         "max" : "150",
         "min" : "25",
         "val" : "25"
      },
      "L2" : {
         "max" : "150",
         "min" : "60",
         "val" : "65"
      },
      "L3" : {
         "max" : "150",
         "min" : "40",
         "val" : "60"
      }
   },
   "tips" : 4,
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg_angle",
                                 "parameters" : {
                                    "Angle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "30"
                                    },
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 1.5707963705062866,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "0"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "1" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg_angle",
                                 "parameters" : {
                                    "Angle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "30"
                                    },
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 1.5707963705062866,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "0"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "2" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg_angle",
                                 "parameters" : {
                                    "Angle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "30"
                                    },
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 1.5707963705062866,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "0"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg_angle",
                                 "parameters" : {
                                    "Angle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "30"
                                    },
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "25"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 1.5707963705062866,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "0"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         }
      },
      "component" : "body_side",
      "parameters" : {
         "Legs" : {
            "max" : "",
            "min" : "",
            "val" : "4"
         },
         "Size" : {
            "max" : "",
            "min" : "",
            "val" : "$BODY"
         },
         "Thickness" : {
            "max" : "",
            "min" : "",
            "val" : "2.2"
         }
      }
   }
}
