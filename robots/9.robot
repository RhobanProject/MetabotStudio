{
   "backend" : "xl-320",
   "parameters" : {
      "L" : {
         "max" : "200",
         "min" : "90",
         "val" : "120"
      },
      "L1" : {
         "max" : "120",
         "min" : "60",
         "val" : "60"
      },
      "L2" : {
         "max" : "100",
         "min" : "30",
         "val" : "35"
      },
      "W" : {
         "max" : "100",
         "min" : "35",
         "val" : "40"
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
                                 "component" : "arm_leg",
                                 "parameters" : {
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L2"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
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
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
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
                     "val" : "25"
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
            "maximum" : 0,
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
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "arm_leg",
                                 "parameters" : {
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L2"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
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
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
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
                     "val" : "25"
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
            "minimum" : 0,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 1,
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
                                 "component" : "arm_leg",
                                 "parameters" : {
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L2"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
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
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
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
                     "val" : "25"
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
            "maximum" : 0,
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
                                 "component" : "arm_leg",
                                 "parameters" : {
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L2"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
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
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
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
                              "val" : "60"
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
                     "val" : "25"
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
            "minimum" : 0,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 1,
            "zero" : 0
         }
      },
      "component" : "square_body",
      "parameters" : {
         "Legs" : {
            "max" : "",
            "min" : "",
            "val" : "4"
         },
         "LegsSpacing" : {
            "max" : "",
            "min" : "",
            "val" : "$L"
         },
         "Thickness" : {
            "max" : "",
            "min" : "",
            "val" : "2.2"
         },
         "Width" : {
            "max" : "",
            "min" : "",
            "val" : "$W"
         }
      }
   }
}
