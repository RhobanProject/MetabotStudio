{
   "backend" : "xl-320",
   "parameters" : {
      "A" : {
         "max" : "45",
         "min" : "-45",
         "val" : "0"
      },
      "BODY" : {
         "max" : "100",
         "min" : "20",
         "val" : "20"
      },
      "L1" : {
         "max" : "150",
         "min" : "45",
         "val" : "40"
      },
      "L2" : {
         "max" : "150",
         "min" : "45",
         "val" : "65"
      },
      "L3" : {
         "max" : "150",
         "min" : "45",
         "val" : "50"
      }
   },
   "tips" : 4,
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
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
                                       "val" : "0"
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
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
                     "maximum" : 0,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_arm",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
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
                                       "val" : "0"
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
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
                     "minimum" : 0,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_arm",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
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
                                       "val" : "0"
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
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
                     "maximum" : 0,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_arm",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$A"
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
                                       "val" : "0"
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
                        "component" : "arm_to_side",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
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
                     "minimum" : 0,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_arm",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
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
