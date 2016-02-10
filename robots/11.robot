{
   "backend" : "xl-320",
   "parameters" : {
      "BODY" : {
         "max" : "100",
         "min" : "20",
         "val" : "20"
      },
      "L1" : {
         "max" : "200",
         "min" : "30",
         "val" : "45"
      },
      "L2" : {
         "max" : "200",
         "min" : "30",
         "val" : "65"
      },
      "L3" : {
         "max" : "200",
         "min" : "30",
         "val" : "75"
      }
   },
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
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
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
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
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
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
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
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
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         },
         "1" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
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
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
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
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
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
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
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
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         },
         "2" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
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
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
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
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
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
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
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
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg_angle",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
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
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "15"
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
                              "orientationY" : 3.1415927410125732,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
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
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
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
            "orientationZ" : 0,
            "remote" : 0,
            "zero" : 0
         }
      },
      "component" : "body",
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
